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
from fixits import ChapelFixit, range_to_text


def name_for_linting(context: Context, node: NamedDecl) -> str:
    name = node.name()

    # Strip dollar signs.
    name = name.replace("$", "")

    # if the name is inside an internal module, strip leading `_` and `chpl_`
    if context.is_bundled_path(node.location().path()):
        if name.startswith("_"):
            name = name[1:]
        if name.startswith("chpl_"):
            name = name[5:]

    return name


def check_camel_case(context: Context, node: NamedDecl):
    return re.fullmatch(
        r"([a-z]+([A-Z][a-z]*|\d+)*|[A-Z]+)?", name_for_linting(context, node)
    )


def check_pascal_case(context: Context, node: NamedDecl):
    return re.fullmatch(
        r"(([A-Z][a-z]*|\d+)+|[A-Z]+)?", name_for_linting(context, node)
    )


def register_rules(driver: LintDriver):
    @driver.basic_rule(VarLikeDecl, default=False)
    def CamelOrPascalCaseVariables(context: Context, node: VarLikeDecl):
        if node.name() == "_":
            return True
        if node.linkage() == "extern":
            return True
        return check_camel_case(context, node) or check_pascal_case(
            context, node
        )

    @driver.basic_rule(Record)
    def CamelCaseRecords(context: Context, node: Record):
        return check_camel_case(context, node)

    @driver.basic_rule(Function)
    def CamelCaseFunctions(context: Context, node: Function):
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

        return check_camel_case(context, node)

    @driver.basic_rule(Class)
    def PascalCaseClasses(context: Context, node: Class):
        return check_pascal_case(context, node)

    @driver.basic_rule(Module)
    def PascalCaseModules(context: Context, node: Module):
        return node.kind() == "implicit" or check_pascal_case(context, node)

    @driver.basic_rule(Module, default=False)
    def UseExplicitModules(_, node: Module):
        return node.kind() != "implicit"

    @driver.basic_rule(Loop)
    @driver.basic_rule(SimpleBlockLike)
    def DoKeywordAndBlock(context: Context, node: typing.Union[Loop, SimpleBlockLike]):
        check = node.block_style() != "unnecessary"
        if not check:
            lines = context.get_file_text(node.location().path()).split("\n")
            text = range_to_text(node.location(), lines)
            # TODO: this should be smarter about the do keyword
            text = re.sub(r"\bdo\s*", "", text, 1)
            return (check, ChapelFixit.build(node.location(), text))

        return (check, None)

    @driver.basic_rule(Coforall, default=False)
    def NestedCoforalls(context: Context, node: Coforall):
        parent = node.parent()
        while parent is not None:
            if isinstance(parent, Coforall):
                return False
            parent = parent.parent()
        return True

    @driver.basic_rule([Conditional, BoolLiteral, chapel.rest])
    def BoolLitInCondStmt(context: Context, node: Conditional):
        lines = context.get_file_text(node.location().path()).split("\n")

        cond = node.condition()
        assert isinstance(cond, BoolLiteral)

        text = None
        if cond.value() == "true":
            text = range_to_text(node.then_block().location(), lines)
        elif cond.value() == "false":
            else_block = node.else_block()
            if else_block is not None:
                text = range_to_text(else_block.location(), lines)
            else:
                text = ""

        if text is not None:
            return (False, ChapelFixit.build(node.location(), text))
        else:
            return False

    @driver.basic_rule(NamedDecl)
    def ChplPrefixReserved(context: Context, node: NamedDecl):
        if node.name().startswith("chpl_"):
            path = node.location().path()
            return context.is_bundled_path(path)
        return True

    @driver.basic_rule(Record)
    @driver.basic_rule(Class)
    def MethodsAfterFields(context: Context, node: typing.Union[Record, Class]):
        method_seen = False
        for child in node:
            if isinstance(child, VarLikeDecl) and method_seen:
                return False
            if isinstance(child, Function):
                method_seen = True
        return True

    # Five things have to match between consecutive decls for this to warn:
    # 1. same type
    # 2. same kind
    # 3. same attributes
    # 4. same linkage
    # 5. same pragmas
    @driver.advanced_rule(default=False)
    def ConsecutiveDecls(_, root: AstNode):
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
            return (
                var_type,
                var_kind,
                var_attributes,
                var_linkage,
                var_pragmas,
            )

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
        prev = None
        for child in root:
            if isinstance(child, Comment):
                continue
            yield from MisleadingIndentation(context, child)

            if prev is not None:
                if child.location().start()[1] == prev.location().start()[1]:
                    yield child

            prev = None
            if isinstance(child, Loop) and child.block_style() == "implicit":
                grandchildren = list(child)
                # safe to access [-1], loops must have at least 1 child
                for blockchild in reversed(list(grandchildren[-1])):
                    if isinstance(blockchild, Comment):
                        continue
                    prev = blockchild
                    break

    @driver.advanced_rule(default=False)
    def UnusedFormal(_, root: AstNode):
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
            yield formals[unused]

    @driver.advanced_rule
    def UnusedLoopIndex(context: Context, root: AstNode):
        indices = dict()
        uses = set()

        def variables(node):
            if isinstance(node, Variable):
                if node.name() != "_":
                    yield node
            elif isinstance(node, TupleDecl):
                for child in node:
                    yield from variables(child)

        for _, match in chapel.each_matching(
            root, [IndexableLoop, ("?decl", Decl), chapel.rest]
        ):
            node = match["decl"]

            for index in variables(node):
                indices[index.unique_id()] = index

        for use, _ in chapel.each_matching(root, Identifier):
            refersto = use.to_node()
            if refersto:
                uses.add(refersto.unique_id())

        # dyno fault if we try to query .location on a Comment
        if not isinstance(root, Comment):
            lines = context.get_file_text(root.location().path()).split("\n")
        for unused in indices.keys() - uses:
            node = indices[unused]
            fixit = None
            parent = node.parent()
            if parent and isinstance(parent, TupleDecl):
                fixit = ChapelFixit.build(node.location(), "_")
            elif parent and isinstance(parent, IndexableLoop):
                if not lines:
                    lines = context.get_file_text(root.location().path()).split(
                        "\n"
                    )
                index_text = range_to_text(node.location(), lines)
                # TODO: this should use loop_header() instead of location()
                text = range_to_text(parent.location(), lines)
                text = re.sub(f"{index_text}\\s+in\\s+", "", text, 1)
                fixit = ChapelFixit.build(parent.location(), text)

            yield (node, fixit)

    @driver.advanced_rule
    def SimpleDomainAsRange(context: Context, root: AstNode):
        lines = None
        # dyno fault if we try to query .location on a Comment
        if not isinstance(root, Comment):
            lines = context.get_file_text(root.location().path()).split("\n")

        for loop, _ in chapel.each_matching(root, IndexableLoop):
            iterand = loop.iterand()
            if not isinstance(iterand, Domain):
                continue
            exprs = list(iterand.exprs())
            if len(exprs) != 1:
                continue

            if not lines:
                lines = context.get_file_text(loop.location().path()).split(
                    "\n"
                )

            s = range_to_text(exprs[0].location(), lines)

            yield (iterand, ChapelFixit.build(iterand.location(), s))

    # @driver.advanced_rule
    # def IncorrectIndentation(context, root):
    #     # TODO
    #     return
