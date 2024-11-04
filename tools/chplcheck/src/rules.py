#
# Copyright 2023-2024 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import re
import typing

import chapel
from chapel import *
from driver import LintDriver
from fixits import Fixit, Edit
from rule_types import BasicRuleResult, AdvancedRuleResult


def variables(node: AstNode):
    if isinstance(node, Variable):
        if node.name() != "_":
            yield node
    elif isinstance(node, TupleDecl):
        for child in node:
            yield from variables(child)


def might_incorrectly_report_location(node: AstNode) -> bool:
    """
    Some Dyno AST nodes do not return locations in the way that we expect.
    For instance, some NamedDecl nodes currently use the name as the location,
    which does not indicate their actual indentation. Rules that depend on
    indentation should leave these variables alone.
    """

    # some NamedDecl nodes currently use the name as the location, which
    # does not indicate their actual indentation.
    #
    # https://github.com/chapel-lang/chapel/issues/25208
    if isinstance(node, (VarLikeDecl, TupleDecl, ForwardingDecl)):
        return True

    # private function locations are bugged and don't include the 'private'
    # keyword.
    #
    # https://github.com/chapel-lang/chapel/issues/24818
    elif isinstance(node, (Function, Use, Import)) and node.visibility() != "":
        return True

    # 'else if' statements do not have proper locations
    #
    # https://github.com/chapel-lang/chapel/issues/25256
    elif isinstance(node, Conditional):
        parent = node.parent()
        grandparent = parent.parent() if parent else None
        if (
            isinstance(parent, Block)
            and parent.block_style() == "implicit"
            and grandparent
            and isinstance(grandparent, Conditional)
        ):
            return True

    return False


def fixit_remove_unused_node(
    node: AstNode,
    lines: Optional[List[str]] = None,
    context: Optional[Context] = None,
) -> Optional[Fixit]:
    """
    Given an unused variable that's either a child of a TupleDecl or an
    iterand in a loop, construct a Fixit that removes it or replaces it
    with '_' as appropriate.

    Expects either a lines list (containing the lines in the file where
    the node is being fixed up) or a context object that will be used to
    determine these lines. Raises if neither is provided.
    """

    parent = node.parent()
    if parent is None:
        return None

    if lines is None:
        if context is None:
            raise ValueError("Either 'lines' or 'context' must be provided")
        else:
            lines = chapel.get_file_lines(context, parent)

    if parent and isinstance(parent, TupleDecl):
        return Fixit.build(Edit.build(node.location(), "_"))
    elif parent and isinstance(parent, IndexableLoop):
        loc = parent.header_location() or parent.location()
        before_loc = loc - node.location()
        after_loc = loc.clamp_left(parent.iterand().location())
        before_lines = range_to_text(before_loc, lines)
        after_lines = range_to_text(after_loc, lines)

        return Fixit.build(Edit.build(loc, before_lines + after_lines))
    return None


def name_for_linting(
    context: Context, node: NamedDecl, internal_prefixes: List[str] = []
) -> str:
    name = node.name()

    # Strip dollar signs.
    name = name.replace("$", "")

    # Strip internal prefixes.
    for p in internal_prefixes:
        if name.startswith(p):
            name = name[len(p) :]
            break

    return name


def check_camel_case(
    context: Context, node: NamedDecl, internal_prefixes: List[str] = []
):
    return re.fullmatch(
        r"([a-z]+([A-Z][a-z]*|\d+)*|[A-Z]+)?",
        name_for_linting(context, node, internal_prefixes),
    )


def check_pascal_case(
    context: Context, node: NamedDecl, internal_prefixes: List[str] = []
):
    return re.fullmatch(
        r"(([A-Z][a-z]*|\d+)+|[A-Z]+)?",
        name_for_linting(context, node, internal_prefixes),
    )


def register_rules(driver: LintDriver):
    @driver.basic_rule(VarLikeDecl, default=False)
    def CamelOrPascalCaseVariables(context: Context, node: VarLikeDecl):
        """
        Warn for variables that are not 'camelCase' or 'PascalCase'.
        """

        if node.name() == "_":
            return True
        if node.linkage() == "extern":
            return True
        internal_prefixes = driver.config.internal_prefixes
        return check_camel_case(
            context, node, internal_prefixes
        ) or check_pascal_case(context, node, internal_prefixes)

    @driver.basic_rule(Record)
    def CamelCaseRecords(context: Context, node: Record):
        """
        Warn for records that are not 'camelCase'.
        """

        internal_prefixes = driver.config.internal_prefixes
        return check_camel_case(context, node, internal_prefixes)

    @driver.basic_rule(Function)
    def CamelCaseFunctions(context: Context, node: Function):
        """
        Warn for functions that are not 'camelCase'.
        """

        # Override functions / methods can't control the name, that's up
        # to the parent.
        if node.is_override():
            return True

        if node.linkage() == "extern":
            return True
        if node.kind() == "operator":
            return True
        if node.name() == "init=":
            return True

        internal_prefixes = driver.config.internal_prefixes
        return check_camel_case(context, node, internal_prefixes)

    @driver.basic_rule(Class)
    def PascalCaseClasses(context: Context, node: Class):
        """
        Warn for classes that are not 'PascalCase'.
        """

        internal_prefixes = driver.config.internal_prefixes
        return check_pascal_case(context, node, internal_prefixes)

    @driver.basic_rule(Module)
    def PascalCaseModules(context: Context, node: Module):
        """
        Warn for modules that are not 'PascalCase'.
        """

        internal_prefixes = driver.config.internal_prefixes
        return node.kind() == "implicit" or check_pascal_case(
            context, node, internal_prefixes
        )

    @driver.basic_rule(Module, default=False)
    def UseExplicitModules(_, node: Module):
        """
        Warn for code that relies on auto-inserted implicit modules.
        """

        return node.kind() != "implicit"

    @driver.basic_rule(Loop)
    @driver.basic_rule(SimpleBlockLike)
    @driver.basic_rule(When)
    def DoKeywordAndBlock(
        context: Context, node: typing.Union[Loop, SimpleBlockLike]
    ):
        """
        Warn for redundant 'do' keyword before a curly brace '{'.
        """

        check = node.block_style() != "unnecessary"
        if not check:
            return BasicRuleResult(node, ignorable=True)
        return check

    @driver.fixit(DoKeywordAndBlock)
    def RemoveUnnecessaryDo(context: Context, result: BasicRuleResult):
        """
        Remove the redundant 'do' keyword before a curly brace '{'.
        """
        node = result.node
        lines = chapel.get_file_lines(context, node)

        if isinstance(node, Loop):
            header_loc = node.header_location()
            if isinstance(node, IndexableLoop) and node.with_clause():
                with_ = node.with_clause()
                assert with_ is not None
                header_loc = header_loc + with_.location()
        else:
            header_loc = node.block_header()
        body_loc = node.curly_braces_location()
        if header_loc is None or body_loc is None:
            return None

        header_text = range_to_text(header_loc, lines)
        body_text = range_to_text(body_loc, lines)

        sep = " "
        if header_loc.end()[0] != body_loc.start()[0]:
            indent = " " * (body_loc.start()[1] - 1)
            sep = "\n" + indent

        new_text = header_text + sep + body_text
        fixit = Fixit.build(Edit.build(node.location(), new_text))
        return fixit

    @driver.basic_rule(set((Loop, Conditional)))
    def ControlFlowParentheses(
        context: Context, node: typing.Union[Loop, Conditional]
    ):
        """
        Warn for unnecessary parentheses in conditional statements and loops.
        """

        subject = None
        if isinstance(node, (DoWhile, While, Conditional)):
            subject = node.condition()
        elif isinstance(node, IndexableLoop):
            subject = node.index()

        # No supported node to examine for redundant parentheses.
        if subject is None:
            return True

        # Tuples need their parentheses.
        if isinstance(subject, Tuple):
            return True

        # No parentheses to speak of
        paren_loc = subject.parenth_location()
        if paren_loc is None:
            return True

        # Now, we should warn: there's a node in a conditional or
        # if/else, it has parentheses at the top level, but it doesn't need them.
        return BasicRuleResult(subject, data=subject)

    @driver.fixit(ControlFlowParentheses)
    def RemoveControlFlowParentheses(context: Context, result: BasicRuleResult):
        # Since we're here, these should already be non-None.
        subject = result.data
        assert subject
        paren_loc = subject.parenth_location()
        assert paren_loc

        # If parentheses span multiple lines, don't provide a fixit,
        # since the indentation would need more thought.
        start_line, start_col = paren_loc.start()
        end_line, end_col = paren_loc.end()
        if start_line != end_line:
            return []

        lines = chapel.get_file_lines(context, result.node)
        new_text = range_to_text(paren_loc, lines)[1:-1]

        start_line_str = lines[start_line - 1]
        end_line_str = lines[end_line - 1]
        # For 'if(x)', can't turn this into 'ifx', need an extra space.
        if start_col > 1 and not start_line_str[start_col - 2].isspace():
            new_text = " " + new_text
        # Similarly, '(x)do', can't turn this into 'xdo', need an extra space.
        if (
            end_col < len(end_line_str)
            and not end_line_str[end_col - 1].isspace()
        ):
            new_text += " "

        return [Fixit.build(Edit.build(paren_loc, new_text))]

    @driver.basic_rule(Coforall, default=False)
    def NestedCoforalls(context: Context, node: Coforall):
        """
        Warn for nested 'coforall' loops, which could lead to performance hits.
        """

        parent = node.parent()
        while parent is not None:
            if isinstance(parent, Coforall):
                return False
            parent = parent.parent()
        return True

    @driver.basic_rule([Conditional, BoolLiteral, chapel.rest])
    def BoolLitInCondStmt(_: Context, node: Conditional):
        """
        Warn for boolean literals like 'true' in a conditional statement.
        """
        return BasicRuleResult(node)

    # TODO: at some point, we should support a fixit that removes the
    # conditions and the braces, but the way locations work right now makes
    # that difficult. Blocks get built and rebuilt in the parser, making it
    # hard to tag the resulting block with curky braces locations.
    @driver.fixit(BoolLitInCondStmt)
    def FixBoolLitInCondStmt_KeepBraces(
        context: Context, result: BasicRuleResult
    ):
        """
        Remove the unused branch of a conditional statement, keeping the braces.
        """
        node = result.node
        lines = chapel.get_file_lines(context, node)

        cond = node.condition()
        assert isinstance(cond, BoolLiteral)

        text = None
        if cond.value():
            text = range_to_text(node.then_block().location(), lines)
        else:
            else_block = node.else_block()
            if else_block is not None:
                text = range_to_text(else_block.location(), lines)
            else:
                text = ""
        # should be set in all branches
        assert text is not None

        return Fixit.build(Edit.build(node.location(), text))

    @driver.basic_rule(NamedDecl)
    def ChplPrefixReserved(context: Context, node: NamedDecl):
        """
        Warn for user-defined names that start with the 'chpl_' reserved prefix.
        """

        if node.name().startswith("chpl_"):
            path = node.location().path()
            return context.is_bundled_path(path)
        return True

    @driver.basic_rule(Record)
    @driver.basic_rule(Class)
    def MethodsAfterFields(context: Context, node: typing.Union[Record, Class]):
        """
        Warn for classes or records that mix field and method definitions.
        """

        method_seen = False
        for child in node:
            if isinstance(child, VarLikeDecl) and method_seen:
                return BasicRuleResult(node, ignorable=True)
            if isinstance(child, Function):
                method_seen = True
        return True

    @driver.basic_rule(EmptyStmt)
    def EmptyStmts(_, node: EmptyStmt):
        """
        Warn for empty statements (i.e., unnecessary semicolons).
        """
        p = node.parent()
        if p and isinstance(p, SimpleBlockLike) and len(list(p.stmts())) == 1:
            # dont warn if the EmptyStmt is the only statement in a block
            return True

        return BasicRuleResult(
            node, fixits=Fixit.build(Edit.build(node.location(), ""))
        )

    @driver.basic_rule(TupleDecl)
    def UnusedTupleUnpack(context: Context, node: TupleDecl):
        """
        Warn for unused tuple unpacking, such as '(_, _)'.
        """

        varset = set(variables(node))
        if len(varset) == 0:
            fixit = fixit_remove_unused_node(node, context=context)
            if fixit is not None:
                return BasicRuleResult(node, fixits=fixit)
            return False
        return True

    @driver.basic_rule(OpCall)
    def ComplexLiteralOrder(context: Context, node: OpCall):
        """
        Warn for complex literals that are not in a consistent order.
        """

        complex_lit = chapel.is_complex_literal(node)
        if not complex_lit:
            return True

        # If the first element is not the imaginary literal, then we have
        # 10+2i, which is the correct order.
        if not isinstance(complex_lit[0], ImagLiteral):
            return True

        # Eventually, auto-fixit will go here. For now, just warn.
        return False

    # Five things have to match between consecutive decls for this to warn:
    # 1. same type
    # 2. same kind
    # 3. same attributes
    # 4. same linkage
    # 5. same pragmas
    @driver.advanced_rule(default=False)
    def ConsecutiveDecls(_, root: AstNode):
        """
        Warn for consecutive variable declarations that can be combined.
        """

        def is_relevant_decl(node):
            var_node = None
            if isinstance(node, MultiDecl):
                for child in node:
                    if isinstance(child, Variable):
                        var_node = child
                else:
                    return None
            elif isinstance(node, Variable):
                var_node = node
            else:
                return None

            var_type = None
            var_type_expr = var_node.type_expression()

            if isinstance(var_type_expr, FnCall):
                # for function call, we need to match all the components
                var_type = ""
                for child in var_type_expr:
                    if child is None:
                        continue
                    if "name" in dir(child):
                        var_type += child.name()
                    elif "text" in dir(child):
                        var_type += child.text()
            elif isinstance(var_type_expr, Identifier):
                var_type = var_type_expr.name()

            var_kind = var_node.kind()

            var_attributes = ""
            var_attribute_group = var_node.attribute_group()
            if var_attribute_group:
                var_attributes = " ".join(
                    [a.name() for a in var_attribute_group if a is not None]
                )

            var_linkage = var_node.linkage()

            var_pragmas = " ".join(var_node.pragmas())
            return var_type, var_kind, var_attributes, var_linkage, var_pragmas

        def recurse(node):
            consecutive = []
            last_characteristics = None

            for child in node:
                # we want to skip Comments entirely
                if isinstance(child, Comment):
                    continue

                # we want to do MultiDecls and TupleDecls, but not recurse
                skip_children = isinstance(child, (MultiDecl, TupleDecl))

                if not skip_children:
                    yield from recurse(child)

                new_characteristics = is_relevant_decl(child)
                compatible = (
                    new_characteristics is not None
                    and new_characteristics == last_characteristics
                )

                last_characteristics = new_characteristics

                if compatible:
                    consecutive.append(child)
                else:
                    # this one doesn't match, yield any from previous sequence
                    # and start looking for matches for this one
                    if len(consecutive) > 1:
                        yield consecutive[1]
                    consecutive = [child]

            if len(consecutive) > 1:
                yield consecutive[1]

        yield from recurse(root)

    @driver.advanced_rule
    def MisleadingIndentation(context: Context, root: AstNode):
        """
        Warn for single-statement blocks that look like they might be multi-statement blocks.
        """
        if isinstance(root, Comment):
            return
        prev = []
        fix = None
        for child in root:
            if isinstance(child, Comment):
                continue
            if might_incorrectly_report_location(child):
                continue
            yield from MisleadingIndentation(context, child)

            if prev and any(
                p.location().start()[1] == child.location().start()[1]
                for p in prev
            ):
                yield AdvancedRuleResult(child, fix)

            prev = []
            fix = append_nested_single_stmt(child, prev)

    def append_nested_single_stmt(node, prev: List[AstNode]):
        if isinstance(node, Loop) and node.block_style() == "implicit":
            children = list(node)
            # safe to access [-1], loops must have at least 1 child
            inblock = reversed(list(children[-1]))
        elif isinstance(node, On) and node.block_style() == "implicit":
            inblock = node.stmts()
        else:
            # Should we also check for Conditionals here?
            return None
        for stmt in inblock:
            if isinstance(stmt, Comment):
                continue
            if might_incorrectly_report_location(stmt):
                continue
            prev.append(stmt)
            append_nested_single_stmt(stmt, prev)
            return node  # Return the outermost on to use an anchor

        return None

    @driver.fixit(MisleadingIndentation)
    def FixMisleadingIndentation(context: Context, result: AdvancedRuleResult):
        """
        Align second statement to be outside of the loop.
        """
        assert isinstance(result.anchor, AstNode)
        prevloop_loc = result.anchor.location()
        loc = result.node.location()
        lines = chapel.get_file_lines(context, result.node)

        fixit = None
        # only apply the fixit when the fix is to indent `child`
        # and `child ` is a single line
        if (
            loc.start()[1] != prevloop_loc.start()[1]
            and loc.start()[0] == loc.end()[0]
        ):
            line_start = (loc.start()[0], 1)
            parent_indent = max(prevloop_loc.start()[1] - 1, 0)
            text = " " * parent_indent + range_to_text(loc, lines)
            fixit = Fixit.build(Edit(loc.path(), line_start, loc.end(), text))
        return [fixit] if fixit else []

    @driver.advanced_rule
    def UnusedFormal(context: Context, root: AstNode):
        """
        Warn for unused formals in functions.
        """
        if isinstance(root, Comment):
            return

        formals = dict()
        uses = set()

        for formal, _ in chapel.each_matching(root, Formal):
            # For now, it's harder to tell if we're ignoring 'this' formals
            # (what about method calls with implicit receiver?). So skip
            # 'this' formals.
            if formal.name() == "this":
                continue

            # extern functions have no bodies that can use their formals.
            if formal.parent().linkage() == "extern":
                continue

            formals[formal.unique_id()] = formal

        for use, _ in chapel.each_matching(root, Identifier):
            refersto = use.to_node()
            if refersto:
                uses.add(refersto.unique_id())

        for unused in formals.keys() - uses:
            anchor = formals[unused].parent()
            yield AdvancedRuleResult(formals[unused], anchor)

    @driver.advanced_rule
    def UnusedTaskIntent(context: Context, root: AstNode):
        """
        Warn for unused task intents in functions.
        """
        if isinstance(root, Comment):
            return

        intents = dict()
        uses = set()

        for intent, _ in chapel.each_matching(
            root, set([TaskVar, ReduceIntent])
        ):
            # task private variables may have side effects,
            # so we don't want to warn on them
            if isinstance(intent, TaskVar):
                if intent.intent() == "var":
                    continue
                if intent.intent() == "<const-var>" and (
                    intent.type_expression() is not None
                    or intent.init_expression() is not None
                ):
                    continue
            intents[intent.unique_id()] = intent

        for use, _ in chapel.each_matching(root, Identifier):
            refersto = use.to_node()
            if refersto:
                uses.add(refersto.unique_id())

        for unused in intents.keys() - uses:
            taskvar = intents[unused]
            with_clause = taskvar.parent()
            task_block = with_clause.parent()

            # only loops can be anchors for attributes
            anchor = None
            if isinstance(task_block, chapel.Loop):
                anchor = task_block
            yield AdvancedRuleResult(
                taskvar, anchor, data=(with_clause, task_block)
            )

    @driver.fixit(UnusedTaskIntent)
    def RemoveTaskIntent(context: Context, result: AdvancedRuleResult):
        """
        Remove the unused task intent from the function.
        """
        assert isinstance(result.data, tuple)
        with_clause, task_block = result.data

        fixit = None
        # if the with clause only has one expr, remove the entire with clause
        if len(list(with_clause.exprs())) == 1:
            with_loc = with_clause.location()
            # header_loc is the location of the block header without the `with`
            # e.g. `forall i in 1..10`, `begin`, `cobegin`
            header_loc = (
                task_block.header_location()
                if isinstance(task_block, Loop)
                else task_block.block_header()
            )
            if header_loc is not None:
                start = header_loc.end()
                end = with_loc.end()
            else:
                start = with_loc.start()
                end = with_loc.end()

            fixit = Fixit.build(Edit(with_loc.path(), start, end, ""))

        else:
            # for now, locations are messy enough that we can't easily cleanly
            # remove the taskvar
            pass
        return [fixit] if fixit else []

    @driver.advanced_rule
    def UnusedTypeQuery(context: Context, root: AstNode):
        """
        Warn for unused type queries in functions.
        """
        if isinstance(root, Comment):
            return

        typequeries = dict()
        uses = set()

        for tq, _ in chapel.each_matching(root, TypeQuery):
            typequeries[tq.unique_id()] = tq

        for use, _ in chapel.each_matching(root, Identifier):
            refersto = use.to_node()
            if refersto:
                uses.add(refersto.unique_id())

        for unused in typequeries.keys() - uses:
            yield AdvancedRuleResult(typequeries[unused])

    @driver.advanced_rule
    def UnusedLoopIndex(context: Context, root: AstNode):
        """
        Warn for unused index variables in loops.
        """
        if isinstance(root, Comment):
            return

        indices = dict()
        uses = set()

        for loop, _ in chapel.each_matching(root, IndexableLoop):
            node = loop.index()
            if node is None:
                continue

            for index in variables(node):
                indices[index.unique_id()] = (index, loop)

        for use, _ in chapel.each_matching(root, Identifier):
            refersto = use.to_node()
            if refersto:
                uses.add(refersto.unique_id())

        lines = chapel.get_file_lines(context, root)
        for unused in indices.keys() - uses:
            node, loop = indices[unused]
            fixit = fixit_remove_unused_node(node, lines)
            fixits = [fixit] if fixit else []
            yield AdvancedRuleResult(node, loop, fixits=fixits)

    @driver.advanced_rule
    def SimpleDomainAsRange(context: Context, root: AstNode):
        """
        Warn for simple domains in loops that can be ranges.
        """
        # dyno fault if we try to query .location on a Comment
        if isinstance(root, Comment):
            return
        lines = chapel.get_file_lines(context, root)

        def is_range_like(node: AstNode):
            """
            a node is range like if its a range, a `count` expr with a
            range-like on the lhs, a `by` expr with a range-like on the lhs, or
            an `align` expr with a range-like on the lhs
            """

            if isinstance(node, Range):
                return True
            if (
                isinstance(node, OpCall)
                and node.is_binary_op()
                and (
                    node.op() == "#"
                    or node.op() == "by"
                    or node.op() == "align"
                )
            ):
                return is_range_like(node.actual(0))
            return False

        for loop, _ in chapel.each_matching(root, IndexableLoop):
            # dont warn for array types
            if isinstance(loop, BracketLoop) and loop.is_maybe_array_type():
                continue
            iterand = loop.iterand()
            if not isinstance(iterand, Domain):
                continue
            exprs = list(iterand.exprs())
            if len(exprs) != 1:
                continue
            # only warn for ranges or count operators
            if not is_range_like(exprs[0]):
                continue

            s = range_to_text(exprs[0].location(), lines)

            fixit = Fixit.build(Edit.build(iterand.location(), s))
            yield AdvancedRuleResult(iterand, anchor=loop, fixits=[fixit])

    @driver.advanced_rule
    def IncorrectIndentation(context: Context, root: AstNode):
        """
        Warn for inconsistent or missing indentation
        """

        # First, recurse and find warnings in children.
        for child in root:
            yield from IncorrectIndentation(context, child)

        def contains_statements(node: AstNode) -> bool:
            """
            Returns true for allow-listed AST nodes that contain
            just a list of statements.
            """
            classes = (
                Record,
                Class,
                Module,
                SimpleBlockLike,
                Interface,
                Union,
                Enum,
                Cobegin,
            )
            return isinstance(node, classes)

        def unwrap_intermediate_block(node: AstNode) -> Optional[AstNode]:
            """
            Given a node, find the reference indentation that its children
            should be compared against.

            This method also rules out certain nodes that should
            not be used for parent-child indentation comparison.
            """
            if not isinstance(node, Block):
                return node

            parent = node.parent()
            if not parent:
                return node

            if isinstance(parent, (Function, Loop)):
                return parent
            elif isinstance(parent, Conditional):
                return None if parent.is_expression_level() else parent
            return node

        # If root is something else (e.g., function call), do not
        # apply indentation rules; only apply them to things that contain
        # a list of statements.
        is_eligible_parent_for_indentation = contains_statements(root)
        if not is_eligible_parent_for_indentation:
            return

        parent_for_indentation = unwrap_intermediate_block(root)
        parent_depth = None
        if parent_for_indentation is None:
            # don't compare against any parent depth.
            pass
        # For implicit modules, proper code will technically be on the same
        # line as the module's body. But we don't want to warn about that,
        # since we don't want to ask all code to be indented one level deeper.
        elif not (
            isinstance(parent_for_indentation, Module)
            and parent_for_indentation.kind() == "implicit"
        ):
            parent_depth = parent_for_indentation.location().start()[1]

        prev = None
        prev_depth = None
        prev_line = None

        # We only care about misaligned statements, so we don't want to do stuff
        # like warn for inherit-exprs or pragmas on a record.
        iterable = root
        if isinstance(root, AggregateDecl):
            iterable = root.decls_or_comments()
        elif isinstance(root, SimpleBlockLike):
            iterable = root.stmts()
        elif isinstance(root, Module) and root.attribute_group() is not None:
            # attribute group is the first child, skip it
            iterable = list(root)[1:]

        for child in iterable:
            if isinstance(child, Comment):
                continue

            if might_incorrectly_report_location(child):
                continue
            # Empty statements get their own warnings, no need to warn here.
            elif isinstance(child, EmptyStmt):
                continue

            line, depth = child.location().start()

            # Warn for two statements on one line:
            #   var x: int; var y: int;
            if line == prev_line:
                # Exception for enums, which are allowed to be on the same line.
                #   enum color { red, green, blue }
                if not isinstance(child, EnumElement):
                    yield child
            # Warn for misaligned siblings:
            #   var x: int;
            #     var y: int;
            elif prev_depth and depth != prev_depth:
                # Special case, slightly coarse: avoid double-warning with
                # MisleadingIndentation
                if not (
                    prev
                    and (isinstance(prev, Loop) or isinstance(prev, On))
                    and prev.block_style() == "implicit"
                ):
                    yield child

                # Do not update 'prev_depth'; use original prev_depth as
                # reference for next sibling.
                prev_line = line
                prev = child
                continue
            # Warn for children that are not indented relative to parent
            #
            #   record r {
            #   var x: int;
            #   }
            elif parent_depth and depth == parent_depth:
                # only loops and NamedDecls can be anchors for indentation
                anchor = (
                    parent_for_indentation
                    if isinstance(parent_for_indentation, (Loop, NamedDecl))
                    else None
                )
                yield AdvancedRuleResult(child, anchor=anchor)

            prev_depth = depth
            prev = child
            prev_line = line

    @driver.advanced_rule
    def MissingInIntent(_, root: chapel.AstNode):
        """
        Warn for formals used to initialize fields that are missing an 'in' intent.
        """
        if isinstance(root, chapel.Comment):
            return

        for agg, _ in chapel.each_matching(root, chapel.AggregateDecl):
            assert isinstance(agg, chapel.AggregateDecl)
            fields: Dict[str, chapel.Variable] = {}
            inits: List[chapel.Function] = []
            for nd in agg:
                if isinstance(nd, chapel.Variable) and nd.is_field():
                    fields[nd.unique_id()] = nd
                if isinstance(nd, chapel.Function) and nd.name() in (
                    "init",
                    "init=",
                ):
                    inits.append(nd)

            for init in inits:
                formals: Dict[str, chapel.Formal] = {}
                for f, _ in chapel.each_matching(init, chapel.Formal):
                    assert isinstance(f, chapel.Formal)
                    if f.intent() != "<default-intent>":
                        continue
                    formals[f.unique_id()] = f

                for stmt in chapel.preorder(init):
                    if isinstance(stmt, chapel.Init):
                        break

                    if not isinstance(stmt, chapel.OpCall):
                        continue

                    if stmt.op() != "=":
                        continue

                    lhs = stmt.actual(0)
                    lhs_is_field = False
                    if isinstance(lhs, (chapel.Identifier, chapel.Dot)):
                        to = lhs.to_node()
                        if to and to.unique_id() in fields:
                            lhs_is_field = True
                    rhs = stmt.actual(1)
                    rhs_is_formal = None
                    if isinstance(rhs, chapel.Identifier):
                        to = rhs.to_node()
                        if to and to.unique_id() in formals:
                            rhs_is_formal = to

                    if lhs_is_field and rhs_is_formal:
                        yield AdvancedRuleResult(rhs_is_formal)

    @driver.fixit(MissingInIntent)
    def FixMissingInIntent(context: Context, result: AdvancedRuleResult):
        """
        Add the 'in' intent to the formal parameter.
        """
        assert isinstance(result.node, chapel.Formal)
        lines = chapel.get_file_lines(context, result.node)

        fixit = Fixit.build(
            Edit.build(
                result.node.location(),
                "in " + range_to_text(result.node.location(), lines),
            )
        )
        return [fixit]
