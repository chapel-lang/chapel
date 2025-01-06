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

"""
=================
Replace Utilities
=================

A set of utilities for performing search-and-replace operations on Chapel code,
informed by Chapel's AST.
"""

import argparse
import chapel
import chapel
import os
import sys
import typing


class ReplacementContext:
    """
    This class is given as an argument to 'finder' functions so that
    they are able to retrieve information about the nodes and the current
    file. While Dyno parses the file and reports locations using line and
    column numbers, it does not have mappings from lines and columns to
    offsets in the file (which is what we need to read text). This class
    contains that information.
    """

    def __init__(self, path: str):
        """
        Given a file path, creates a ReplacementContext that contains the
        mapping information for that file.
        """

        # Build the line number -> file position map
        with open(path, "r") as file:
            self.content = file.read()
        self.lines = {1: 0}
        self.lines_back = {}
        line = 1
        for i, char in enumerate(self.content):
            self.lines_back[i] = line
            if char == "\n":
                line += 1
                self.lines[line] = (
                    i + 1
                )  # the next characrer is the start of the next line

    def loc_to_idx(self, loc: typing.Tuple[int, int]) -> int:
        """
        Given a location (as retrieved from an AST node), convert this
        location into an offset in the source file.
        """
        (row, col) = loc
        return self.lines[row] + (col - 1)

    def node_idx_range(self, node: chapel.AstNode) -> typing.Tuple[int, int]:
        """
        Given a node, determine where it starts and ends in the given source
        file.
        """
        loc = node.location()

        range_start = self.loc_to_idx(loc.start())
        range_end = self.loc_to_idx(loc.end())
        return (range_start, range_end)

    def node_exact_string(self, node: chapel.AstNode) -> str:
        """
        Return the substring that corresponds to the given node in the source
        file.
        """
        (range_start, range_end) = self.node_idx_range(node)
        return self.content[range_start:range_end]

    def node_indent(self, node: chapel.AstNode) -> int:
        """
        Determine the number of characters between the given node and the
        beginning of the line.
        """
        (range_start, _) = self.node_idx_range(node)
        return range_start - self.lines[self.lines_back[range_start]]


def rename_formals(
    rc: ReplacementContext, fn: chapel.Function, renames: typing.Dict[str, str]
):
    """
    Helper iterator to be used in finder functions. Given a function
    and a map of ('original formal name' -> 'new formal name'), yields
    updates that perform the formal renaming.
    """

    def name_replacer(name):
        return lambda child_text: child_text.replace(name, renames[name])

    for child in fn.formals():
        name = child.name()
        if name not in renames:
            continue

        yield (child, name_replacer(name))


def rename_named_actuals(
    rc: ReplacementContext, call: chapel.Call, renames: typing.Dict[str, str]
):
    """
    Helper iterator to be used in finder functions. Given a function call expression,
    and a map of ('original name' -> 'new name'), yields
    updates that rename named actuals like the `x` in `f(x=...)`.
    """
    for actual in call.actuals():
        if isinstance(actual, tuple):
            (name, actual) = actual
            if name not in renames:
                continue

            actual_text = rc.node_exact_string(actual)

            # TODO: but there's no node that includes the name = in its location...
            # yield (actual, actual_text.replace(name, renames[name]))
            yield from []


def replace(
    finder: typing.Generator, ctx: chapel.Context, filename: str
) -> str:
    """
    Drives replacement of text based on matches found in `finder`.
    """
    asts = ctx.parse(filename)
    rc = ReplacementContext(filename)
    new_content = rc.content

    # First, store all the replacements in a map; then, walk the tree in a
    # reverse-postorder traversal (child nodes in reverse order, then parent)
    # and apply the transformations.

    nodes_to_replace = {}

    def compose(outer, inner):
        return lambda text: outer(inner(text))

    for ast in asts:
        for node, replace_with in finder(rc, ast):
            uid = node.unique_id()
            # Old result doesn't matter or doesn't exist; throw it out.
            if not callable(replace_with) or uid not in nodes_to_replace:
                nodes_to_replace[uid] = replace_with
            # replace_with is a callable, which means it transforms the previous
            # text. See if a transformation for this node has already been
            # requested.
            elif uid in nodes_to_replace:
                # Old substitution is also a callable; need to create composition.
                if callable(nodes_to_replace[uid]):
                    nodes_to_replace[uid] = compose(
                        replace_with, nodes_to_replace[uid]
                    )
                # Old substitution is a string; we can apply the callable to get
                # another string.
                else:
                    nodes_to_replace[uid] = replace_with(nodes_to_replace[uid])

    def recurse(node: chapel.AstNode):
        my_replace = None
        if node.unique_id() in nodes_to_replace:
            my_replace = nodes_to_replace[node.unique_id()]

        # If we don't have our own substitution, we can just propagate the
        # child substitutions.
        if my_replace is None:
            for child in reversed(list(node)):
                yield from recurse(child)
        # If it's not callable, it must be a string; we don't care about child
        # replacements, since our own target is constant.
        elif not callable(my_replace):
            (replace_from, replace_to) = rc.node_idx_range(node)
            yield (replace_from, replace_to, my_replace)

        # We have a callable replacement, which means we should apply child
        # substitutions to our text and then call the replacement with that.
        else:
            (replace_from, replace_to) = rc.node_idx_range(node)
            my_text = rc.node_exact_string(node)
            for child in reversed(list(node)):
                for child_from, child_to, child_str in recurse(child):
                    # Child is not inside this node, so it can be replaced as before
                    if child_from >= replace_to:
                        yield (child_from, child_to, child_str)

                    # Otherwise, child is inside, and we have to apply the patch
                    # to our own content before handing it to the transformer.
                    else:
                        child_from -= replace_from
                        child_to -= replace_from

                        my_text = (
                            my_text[:child_from]
                            + child_str
                            + my_text[child_to:]
                        )
            yield (replace_from, replace_to, my_replace(my_text))

    for ast in reversed(asts):
        for replace_from, replace_to, replace_with in recurse(ast):
            new_content = (
                new_content[:replace_from]
                + replace_with
                + new_content[replace_to:]
            )

    return new_content


def _do_replace(
    finder: typing.Generator,
    ctx: chapel.Context,
    filename: str,
    suffix: str,
    inplace: bool,
):

    new_content = replace(finder, ctx, filename)

    if inplace:
        store_into = filename
    else:
        store_into = filename + suffix
    with open(store_into, "w") as newfile:
        newfile.write(new_content)


def run(
    finder: typing.Generator,
    name: str = "replace",
    description: str = "A tool to search-and-replace Chapel expressions with others",
):
    """
    Start a command-line replacer program with the given 'finder' function.
    This program will automatically support accepting a list of files on
    the command line, and two command-line options.

    The first option is '--suffix', used for out-of-place substitutions: the
    new file will be called '.chpl.thesuffix'. The default suffix value is 'new'.

    The second option is '--in-place', used to perform in-place (modifying)
    substitutions on files.
    """

    parser = argparse.ArgumentParser(prog=name, description=description)
    parser.add_argument("filenames", nargs="*")
    parser.add_argument(
        "--suffix", dest="suffix", action="store", default=".new"
    )
    parser.add_argument(
        "--in-place", dest="inplace", action="store_true", default=False
    )
    args = parser.parse_args()

    for filename, ctx in chapel.files_with_contexts(args.filenames):
        _do_replace(finder, ctx, filename, args.suffix, args.inplace)


def fuse(*args):
    """
    Combines multiple 'finder' iterators into one.
    """

    def fused(rc, root):
        for arg in args:
            yield from arg(rc, root)

    return fused
