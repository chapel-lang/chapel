from chapel import (
    AggregateDecl,
    AstNode,
    Block,
    Class,
    Cobegin,
    Comment,
    Conditional,
    Enum,
    EnumElement,
    EmptyStmt,
    Function,
    Interface,
    Loop,
    Module,
    NamedDecl,
    On,
    Record,
    SimpleBlockLike,
    Union,
)
from typing import Optional, List, Tuple


class IndentationCollector:
    def __init__(self):
        self.misleadingly_indented_groups: dict[AstNode, Tuple[AstNode, Optional[AstNode], List[AstNode]]] = {}
        self.incorrectly_indented_nodes: dict[AstNode, Optional[AstNode]] = {}

    def _append_nested_single_stmt(self, node: AstNode, prev: List[Tuple[AstNode, AstNode, List[AstNode]]], anchor: Optional[AstNode] = None) -> Optional[AstNode]:
        if not anchor:
            anchor = node

        if isinstance(node, Loop) and node.block_style() == "implicit":
            children = list(node)
            # safe to access [-1], loops must have at least 1 child
            inblock = reversed(list(children[-1]))
        elif isinstance(node, On) and node.block_style() == "implicit":
            inblock = node.stmts()
        else:
            return

        for stmt in inblock:
            if isinstance(stmt, Comment):
                continue
            prev.append((stmt, anchor, []))
            self._append_nested_single_stmt(stmt, prev, anchor)
            break

    def _contains_statements(self, node: AstNode) -> bool:
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

    def _unwrap_intermediate_block(self, node: AstNode) -> Optional[AstNode]:
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

    def _is_block_in_else_if_chain(
        self, block: AstNode, parent: Optional[AstNode]
    ) -> bool:
        return (
            isinstance(parent, Conditional)
            and parent.has_else_block()
            and parent.num_else_stmts() == 1
            and parent.else_stmt(0).unique_id() == block.unique_id()
            and parent.else_block_style() == "implicit"
        )

    def _find_anchor(self, node: Optional[AstNode]) -> Optional[AstNode]:
        # only loops and NamedDecls can be anchors for indentation
        anchor = node if isinstance(node, (Loop, NamedDecl)) else None
        if isinstance(node, Module) and node.kind() == "implicit":
            anchor = None
        return anchor

    def _note_misleading_group(self, misleading_child: AstNode, anchor: Optional[AstNode], group: List[AstNode]):
        for member in group:
            self.misleadingly_indented_groups[member] = (misleading_child, anchor, group)

    def _note_incorrectly_indented(self, child: AstNode, anchor: Optional[AstNode]):
        self.incorrectly_indented_nodes[child] = anchor

    def collect(self, root: AstNode):
        if isinstance(root, Comment):
            return

        # First, recurse and find warnings in children.
        for child in root:
            self.collect(child)

        # If root is something else (e.g., function call), do not
        # apply indentation rules; only apply them to things that contain
        # a list of statements.
        is_eligible_parent_for_indentation = self._contains_statements(root)

        parent_for_indentation = self._unwrap_intermediate_block(root)
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

        prev_depth = None
        prev_line = None
        prev_single_stmts: List[Tuple[AstNode, AstNode, List[AstNode]]] = []
        misleading_indent_child = None

        # We only care about misaligned statements, so we don't want to do stuff
        # like warn for inherit-exprs or pragmas on a record.
        iterable = root
        if isinstance(root, AggregateDecl):
            iterable = root.decls_or_comments()
        elif isinstance(root, SimpleBlockLike):
            iterable = root.stmts()
        elif isinstance(root, Interface):
            iterable = root.stmts()
        elif isinstance(root, Cobegin):
            iterable = root.task_bodies()
        elif (
            isinstance(root, (Module, Enum))
            and root.attribute_group() is not None
        ):
            # attribute group is the first child, skip it
            iterable = list(root)[1:]

        for child in iterable:
            if isinstance(child, Comment):
                continue

            # Empty statements get their own warnings, no need to warn here.
            if isinstance(child, EmptyStmt):
                continue

            # Check if the previous statement ended in a single-statement
            # 'do' and the current statement is on the same line, which
            # suggests they are meant to be part of the same block, but aren't.
            line, depth = child.location().start()

            while prev_single_stmts:
                p, anchor, group = prev_single_stmts[-1]
                p_depth = p.location().start()[1]

                if p_depth > depth:
                    prev_single_stmts.pop()
                    self._note_misleading_group(p, anchor, group)
                    continue

                elif p_depth == depth:
                    group.append(child)

                break

            # If this node is itself an implicit-block loop, add its statements
            # to the stack.
            self._append_nested_single_stmt(child, prev_single_stmts)

            # Warn for two statements on one line:
            #   var x: int; var y: int;
            if line == prev_line:
                # Exception for enums, which are allowed to be on the same line.
                #   enum color { red, green, blue }
                if is_eligible_parent_for_indentation and not isinstance(child, EnumElement):
                    self._note_incorrectly_indented(child, self._find_anchor(parent_for_indentation))

            # Warn for misaligned siblings:
            #   var x: int;
            #     var y: int;
            elif prev_depth and depth != prev_depth:
                if is_eligible_parent_for_indentation:
                    self._note_incorrectly_indented(child, self._find_anchor(parent_for_indentation))

                # Do not update 'prev_depth'; use original prev_depth as
                # reference for next sibling.
                prev_line = line
                continue

            # Warn for children that are not indented relative to parent
            #
            #   record r {
            #   var x: int;
            #   }
            elif is_eligible_parent_for_indentation and parent_depth and depth == parent_depth:
                if self._is_block_in_else_if_chain(child, parent_for_indentation):
                    # don't warn if the child is the only statement in an else implicit block
                    prev_line = line
                    continue
                self._note_incorrectly_indented(child, self._find_anchor(parent_for_indentation))

            prev_depth = depth
            prev_line = line

        for (p, anchor, group) in prev_single_stmts:
            self._note_misleading_group(p, anchor, group)
