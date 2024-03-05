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

from . import core
from .core import *
from collections import defaultdict
import os
from typing import Dict, List, Optional
import typing
from . import visitor

QualifiedType = typing.Tuple[str, Optional[ChapelType], Optional[Param]]

def preorder(node):
    """
    Recursively visit the given AST node, going in pre-order (parent-then-children)
    """
    yield node
    for child in node:
        yield from preorder(child)


def postorder(node):
    """
    Recursively visit the given AST node, going in post-order (children-then-parent)
    """
    for child in node:
        yield from postorder(child)
    yield node


def is_deprecated(node: AstNode) -> bool:
    """
    Returns true if node is marked with a @deprecated attribute
    """
    attrs = node.attribute_group()
    if attrs:
        return attrs.is_deprecated()
    return False


def is_unstable(node: AstNode) -> bool:
    """
    Returns true if node is marked with a @unstable attribute
    """
    attrs = node.attribute_group()
    if attrs:
        return attrs.is_unstable()
    return False


def is_docstring_comment(comment: Comment) -> bool:
    """
    comment is a docstring if it doesn't begin with '//'
    """
    return not comment.text().startswith("//")


class SiblingMap:
    """
    Represents a mapping of nodes to their siblings
    """

    @visitor.visitor
    class SiblingVisitor:
        def __init__(self):
            self.stack: List[Optional[AstNode]] = [None]
            self.map: Dict[str, AstNode] = {}

        @visitor.enter
        def enter_AstNode(self, node: AstNode):
            if len(self.stack) > 0:
                peeked = self.stack[-1]
                if peeked:
                    self.map[node.unique_id()] = peeked
            self.stack.append(None)

        @visitor.exit
        def exit_AstNode(self, node: AstNode):
            self.stack.pop()
            if len(self.stack) > 0:
                self.stack[-1] = node

    def __init__(self, top_level_modules: List[AstNode]):
        vis = SiblingMap.SiblingVisitor()
        for m in top_level_modules:
            vis.visit(m)
        self.siblings = vis.map

    def get_sibling(self, node: AstNode) -> Optional[AstNode]:
        return self.siblings.get(node.unique_id(), None)


def get_docstring(node: AstNode, sibling_map: SiblingMap) -> Optional[str]:
    """
    Get the docstring for a node, if it exists
    """
    prev_sibling = sibling_map.get_sibling(node)
    if (
        prev_sibling
        and isinstance(prev_sibling, Comment)
        and is_docstring_comment(prev_sibling)
    ):
        return prev_sibling.text().lstrip().lstrip("/*").rstrip("*/")
    return None


def parse_attribute(attr, attribute):
    """
    Given an Attribute AST node, and a description of the attribute in
    the form (name, formal_names), return a mapping of formal names to their
    values in the attribute call. Raise errors if the call is malformed in
    some way.
    """

    (name, formals) = attribute

    if attr.name() != name:
        return None

    parse_result = {}

    def save_kw(actual_tuple):
        (name, value) = actual_tuple
        if name not in formals:
            raise Exception("Invalid named argument for attribute!")
        elif name in parse_result:
            raise Exception("Duplicate occurrence of named argument!")
        else:
            parse_result[name] = value

    # First, process arguments in order
    actuals = attr.actuals()
    for actual, formal in zip(actuals, formals):
        # If we found the first named argument, save it and stop
        # processing positional arguments.
        if isinstance(actual, tuple):
            save_kw(actual)
            break

        # Otherwise, it's a positional argument, save it under the current formal.
        parse_result[formal] = actual

    # Finish up with the remaining kwargs, if any.
    for actual in actuals:
        if not isinstance(actual, tuple):
            raise Exception("Mixing named and positional arguments!")
        save_kw(actual)

    for formal in formals:
        if formal not in parse_result:
            parse_result[formal] = None

    return parse_result


rest = "rest"


def match_pattern(ast, pattern):
    """
    Matches the given AST against the given pattern. Patterns are specified
    as AST node classes, strings, or lists thereof. Here's a summary:

    A single node with any number of children:

      chapel.Nodetype

    Any node, saved in a variable:

      "?x"

    Any node at all, not saved in a variable:

      ""

    A single node with any number of children, saved in a variable:

      ("?x", chapel.Nodetype)

    A single node with no children at all:

      [chapel.Nodetype]

    A single node with no children, saved in a variable:

      ["?x", chapel.Nodetype]

    A single node with two children, expressed as patterns P1 and P2:

      [chapel.Nodetype, P1, P2]

    A single node with at least two children:

      [chapel.Nodetype, P1, P2, rest]

    An addition node in which both arguments are equal:

      [chapel.OpCall, "?x", "x"]

    """

    counts = {}

    def fresh(metavar):
        new_var = metavar
        next_index = 0
        if metavar in counts:
            new_var = metavar + str(counts[metavar])
            next_index = counts[metavar] + 1
        counts[metavar] = next_index
        return new_var

    def current(metavar):
        if metavar not in counts:
            # Varaible was never created; it's an error.
            raise Exception("Invalid variable in pattern")

        count = counts[metavar]
        return metavar if count == 0 else metavar + str(count - 1)

    def check_var(ast, pat, variables: Dict):
        # Empty pattern is wildcard
        if len(pat) == 0:
            return True

        if pat[0] == "?":
            # Fresh variable, guaranteed to match.
            variables[fresh(pat[1:])] = ast
            return True
        else:
            # Re-used variable; need to compare for equality.
            variable = current(pat)
            if variable not in variables:
                # Referring to a not-yet-bound variable.
                return False

            # Can't compare ASTs yet
            print("Equality constrait:", variables[variable], ast)
            return True

    def match_inner(ast, pat, variables: Dict) -> bool:
        if isinstance(pat, str):
            return check_var(ast, pat, variables)
        elif isinstance(pat, tuple):
            (pat_name, node_type) = pat

            if not isinstance(ast, node_type):
                return False
            if not check_var(ast, pat_name, variables):
                return False

            return True
        elif isinstance(pat, list):
            idx = 0
            node_type = pat[idx]
            pat_name = None
            if isinstance(node_type, str):
                pat_name = node_type
                idx = 1
                node_type = pat[idx]

            if not isinstance(ast, node_type):
                return False
            if pat_name is not None and not check_var(ast, pat_name, variables):
                return False

            idx += 1
            children = list(ast)
            for child in children:
                # Too many children, pattern didn't expect
                if idx == len(pat):
                    return False

                child_pat = pat[idx]
                if child_pat == "rest":
                    # Special case rest pattern; subsequent children allowed.
                    break
                if not match_inner(child, child_pat, variables):
                    return False
                idx += 1
            else:
                # Did not encounter a rest pattern.

                # If we didn't make it through all the expect patterns, we
                # ran out of children.
                if idx < len(pat) and pat[idx] != "rest":
                    return False

            return True
        elif isinstance(pat, set):
            # check if any of patterns in the set match
            for p in pat:
                local_variables = variables.copy()
                if match_inner(ast, p, local_variables):
                    variables.update(local_variables)
                    return True
            return False
        elif issubclass(pat, AstNode):
            # Just check if the AST node matches
            return isinstance(ast, pat)
        else:
            raise Exception("Invalid pattern!")

    variables = {}
    return variables if match_inner(ast, pattern, variables) else None


def each_matching(node, pattern, iterator=preorder):
    for child in iterator(node):
        variables = match_pattern(child, pattern)
        if variables is not None:
            yield (child, variables)


def files_with_contexts(files):
    """
    Some files might have the same name, which Dyno really doesn't like.
    Stratify files into "buckets"; within each bucket, all filenames are
    unique. Between each bucket, re-create the Dyno context to avoid giving
    it conflicting files.

    Yields files from the argument, as well as the context created for them.
    """

    basenames = defaultdict(lambda: 0)
    buckets = defaultdict(lambda: [])
    for filename in files:
        filename = os.path.realpath(os.path.expandvars(filename))

        basename = os.path.basename(filename)
        bucket = basenames[basename]
        basenames[basename] += 1
        buckets[bucket].append(filename)

    for bucket in buckets:
        ctx = Context()
        to_yield = buckets[bucket]

        for filename in to_yield:
            yield (filename, ctx)
