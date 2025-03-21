#
# Copyright 2020-2025 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
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
This script is used to parse and identify all undocumented symbols in Chapel
source code. It can be passed any number of Chapel source files or directories,
which it will recurse looking for Chapel source files by default.

Disclaimer: this script attempts to be as faithful as possible to the
`chpldoc.cpp` implementation, but may differ slightly in behavior.

The script works on the following symbols
- Modules
- Functions
  - In this sense, a function is anything declared with `proc`, `iter`, or
    `operator`
  - This includes functions defined inside of Modules, AggregateDecls, and
    Interfaces
  - This will not match nested Functions
- AggregateDecls
  - AggregateDecls are Records, Classes, or Unions
  - This will match nested definitions in Modules and AggregateDecls,
    e.g. a Record inside a Record
- Decls
  - Decls are top level variables defined in Modules or AggregateDecls
- MultiDecls
  - Same as Decls, but multiple symbols share the same keyword, e.g.
    `var a, b = 10;`
- Enums
  - Will also match against EnumElements
  - This will match nested definitions in Modules and AggregateDecls
- Interfaces
  - This will match nested definitions in Modules and AggregateDecls

The script pulls out all of these symbols from a parsed source file, and then
determines if the symbol is documented. A symbol is considered documented if
the previous sibling node in the AST is a docstring Comment.

Symbols can be marked 'nodoc', meaning it is ok for the docstring to be
missing. This script considers a symbol to be marked 'nodoc' if any of the
following is true:
- If it is explicitly marked with an attribute `@chpldoc.nodoc`
- If its name is prefixed with `chpl_`
  - This also checks the `pragma "chpldoc ignore chpl prefix"`
- If the symbol is marked private
- If the symbol has a parent that is marked 'nodoc'

This script also provides two optional flags, `--ignore-[unstable|deprecated]`.
If a symbol or its parent was marked with with an `@unstable` or `@deprecated`
attribute, the respective option will cause the script to ignore it. Other ways
of marking symbols unstable or deprecated are not handled by these options.
"""

from collections import defaultdict
from typing import Generator, List
import sys
import os
import argparse as ap
import itertools
import glob
import chapel
from chapel import each_matching, files_with_contexts


"""
helpers
"""


def get_module(n: chapel.AstNode) -> chapel.AstNode:
    if n is None:
        return None
    if isinstance(n, chapel.Module):
        return n
    parent = n.parent()
    if parent:
        return get_module(parent)
    return None


def node_has_attribute(node: chapel.AstNode, marker: str) -> bool:
    """
    a symbol has an attribute if it has an attribute that matches the argument
    """
    attrs = node.attribute_group()
    if attrs:
        return any(a.name() == marker for a in attrs)


def is_deprecated(node: chapel.AstNode) -> bool:
    """a node is deprecated if it or its parent has an attribute that is deprecated"""
    return node_has_attribute(node, "deprecated")


def is_unstable(node: chapel.AstNode) -> bool:
    """a node is unstable if it or its parent has an attribute that is unstable"""
    return node_has_attribute(node, "unstable")


def is_nodoc(node: chapel.AstNode) -> bool:
    """
    a symbol is marked nodoc for the following reasons
    - symbol has attribute `@chpldoc.nodoc`
    - symbol name is prefixed with `chpl_` and symbol does not have `pragma "chpldoc ignore chpl prefix"`
    - symbol is private
    """
    if node_has_attribute(node, "chpldoc.nodoc"):
        return True

    if (
        hasattr(node, "name")
        and node.name().startswith("chpl_")
        and "chpldoc ignore chpl prefix" not in node.pragmas()
    ):
        return True

    # MultiDecl apparently supports comments inside of it
    # so it requires the extra `hasattr` check
    #
    # Note that this check will skip a multi_decl like `var chpl_x = a, y = b;`,
    # even though arguably `y` is documentable. This is an uncommon scenario
    # and one `chpldoc` also doesn't handle currently
    if (
        isinstance(node, chapel.MultiDecl)
        and any(
            n.name().startswith("chpl_") for n in node if hasattr(n, "name")
        )
        and "chpldoc ignore chpl prefix" not in node.pragmas()
    ):
        return True

    if hasattr(node, "visibility") and node.visibility() == "private":
        return True

    return False


def is_docstring_comment(c: chapel.Comment) -> bool:
    """c is a docstring if it doesn't begin with '//'"""
    return not c.text().startswith("//")


def get_node_name(node: chapel.AstNode) -> List[str]:
    """
    get the name of a node, even when it may not have a `name()` method

    if this is a member of a aggregate type, also gets that name
    if this is a member of a module, also gets that name

    For a multidecl, gets all the names

    """

    def get_simple_node_names(node: chapel.AstNode) -> List[str]:
        names: List[str] = []
        if hasattr(node, "name"):
            names.append(node.name())
        elif isinstance(node, chapel.MultiDecl):
            names.extend(c.name() for c in node)
        else:
            names.append(str(node))
        return names

    def get_single_name(node: chapel.AstNode) -> str:
        names = get_simple_node_names(node)
        if len(names) != 1:
            print("Error: violated invariant")
            exit(1)
        return names[0]

    names_to_return = []

    base_names = get_simple_node_names(node)
    for name in base_names:
        # handles secondary methods
        if (
            isinstance(node, chapel.Function)
            and node.this_formal()
            and node.this_formal().type_expression()
        ):
            aggregate_name = get_single_name(node.this_formal().type_expression())
            name = f"{aggregate_name}.{name}"
        # handles primary methods and fields
        elif node.parent() and (
            isinstance(node.parent(), chapel.AggregateDecl)
            or isinstance(node.parent(), chapel.Interface)
        ):
            aggregate_name = get_single_name(node.parent())
            name = f"{aggregate_name}.{name}"

        mod = get_module(node.parent())
        if mod and mod.kind() != "implicit":
            module_name = mod.name()
            name = f"{module_name}.{name}"

        names_to_return.append(name)

    return names_to_return


def look_ahead(iterable, window=1):
    items, nexts = itertools.tee(iterable, 2)
    nexts = itertools.islice(nexts, window, None)
    return itertools.zip_longest(items, nexts)


"""
main code
"""


class FindUndocumentedSymbols:
    def __init__(
        self,
        asts: List[chapel.AstNode],
        ignore_deprecated: bool = False,
        ignore_unstable: bool = False,
    ):
        self.asts = asts
        self.ignore_deprecated = ignore_deprecated
        self.ignore_unstable = ignore_unstable

    def _parent_is_module_or_aggregate(node: chapel.AstNode, match):
        return isinstance(node.parent(), (chapel.Module, chapel.AggregateDecl))

    # (pattern, extra check fun)
    documentable_symbol_patterns = {
        "function": (
            chapel.Function,
            lambda node, match: isinstance(
                node.parent(), (chapel.Module, chapel.AggregateDecl, chapel.Interface)
            ),
        ),
        "module": (chapel.Module, lambda node, match: node.kind() != "implicit"),
        "aggregate_decl": (
            chapel.AggregateDecl,
            _parent_is_module_or_aggregate,
        ),
        "decl": (
            chapel.Variable,
            _parent_is_module_or_aggregate,
        ),
        "multi_decl": (
            chapel.MultiDecl,
            _parent_is_module_or_aggregate,
        ),
        "enum": (
            chapel.Enum,
            _parent_is_module_or_aggregate,
        ),
        "enum_element": (chapel.EnumElement, None),
        "interface": (
            chapel.Interface,
            _parent_is_module_or_aggregate,
        ),
    }

    def _get_previous_sibling(self, node: chapel.AstNode):
        parent = node.parent()
        lookin = parent if parent else self.asts
        for sib1, sib2 in look_ahead(lookin):
            if sib2 and node.unique_id() == sib2.unique_id():
                return sib1
        return None

    def get_documentable_symbols(self) -> Generator:
        for a in self.asts:
            yield from self._get_documentable_symbols(a)

    def _get_documentable_symbols(self, root: chapel.AstNode) -> Generator:
        def _preorder(node):
            """
            this preorder function respects `--ignore-[deprecated|unstable]`
            and checks for nodoc
            """
            if self.ignore_deprecated and is_deprecated(node):
                return
            if self.ignore_unstable and is_unstable(node):
                return
            if is_nodoc(node):
                return
            yield node
            for child in node:
                yield from _preorder(child)

        for (
            pat,
            check_func,
        ) in FindUndocumentedSymbols.documentable_symbol_patterns.values():
            matches = [
                m
                for m in each_matching(root, pat, iterator=_preorder)
                if not check_func or check_func(*m)
            ]
            for node, _ in matches:
                yield node

    def has_doc_comment(self, node: chapel.AstNode) -> bool:
        """
        This node has a doc comment if
        - the previous node is a comment
        """

        prev = self._get_previous_sibling(node)
        if (
            prev
            and isinstance(prev, chapel.Comment)
            and is_docstring_comment(prev)
        ):
            return True

        return False

    def __call__(self):
        for s in self.get_documentable_symbols():
            if not self.has_doc_comment(s):
                yield s


def get_files(files: List[str]) -> Generator:
    """
    Yield all chapel files, following directories recursively
    """

    for file in files:
        if os.path.isfile(file):
            yield file
        elif os.path.isdir(file):
            yield from glob.glob(
                os.path.join(file, "**", "*.chpl"), recursive=True
            )
        else:
            print("Error: path was not file or directory", file=sys.stderr)
            exit(1)


def main(raw_args: List[str]) -> int:
    a = ap.ArgumentParser()
    a.add_argument(
        "files",
        nargs="*",
        help="a list of Chapel files and/or directories to be searched",
    )
    a.add_argument(
        "--ignore-deprecated",
        action="store_true",
        default=False,
        help="don't report warnings for deprecated symbols",
    )
    a.add_argument(
        "--ignore-unstable",
        action="store_true",
        default=False,
        help="don't report warnings for unstable symbols",
    )
    # hidden option, converts warnings to errors to be used in a ci
    a.add_argument("--ci", action="store_true", default=False, help=ap.SUPPRESS)

    args = a.parse_args(raw_args)
    flags = vars(args)
    files = flags.pop("files")
    ci = flags.pop("ci")

    report_kind = "warning" if not ci else "error"
    ret_code = 0

    curdir = os.path.abspath(os.path.curdir)

    for filename, ctx in files_with_contexts(get_files(files)):
        astList = ctx.parse(filename)
        fus = FindUndocumentedSymbols(astList, **flags)
        # sort by line number
        syms = sorted(fus(), key=lambda s: s.location().start())
        for sym in syms:
            loc = sym.location()
            (line, col) = loc.start()
            path = os.path.relpath(loc.path(), curdir)
            names = get_node_name(sym)
            for name in names:
                print(
                    f"{report_kind}: '{name}' at {path}:{line} is undocumented"
                )
                ret_code = 1 if ci else 0

    return ret_code


if __name__ == "__main__":
    exit(main(sys.argv[1:]))
