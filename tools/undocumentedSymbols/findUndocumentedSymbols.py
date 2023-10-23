#
# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

The script works on the following symbols - Modules - Functions
  - In this sense, a function is anything declared with `proc`, `iter`, or
    `operator`
  - This includes functions defined inside of Modules, AggregateDecls, and
    Interfaces
  - This will not match nested Functions
- AggregateDecls - AggregateDecls are Records, Classes, or Unions - This will
  match nested definitions in Modules and AggregateDecls, e.g. a Record inside
  a Record
- Decls - Decls are top level variables defined in Modules or AggregateDecls
- MultiDecls - Same as Decls, but multiple symbols share the same keyword, e.g.
  `var a, b = 10;`
- Enums - Will also match against EnumElements - This will match nested
  definitions in Modules and AggregateDecls
- Interfaces - This will match nested definitions in Modules and AggregateDecls

The script pulls out all of these symbols from a parsed source file, and then
determines if the symbol is documented. A symbol is considered documented if
the previous sibling node in the AST is a docstring Comment.

Symbols can be marked 'nodoc', meaning it is ok for the docstring to be
missing. This script considers a symbol to be marked 'nodoc' based on the
following criteria: - If it is explicitly marked with an attribute
`@chpldoc.nodoc` - If its names is prefixed with `chpl_`
  - This also checks the `pragma "chpldoc ignore chpl prefix"`
- If the symbol is marked private
- If the symbol has a parent that is marked 'nodoc'

This script also provides two optional flags, `--ignore-[unstable|deprecated]`.
These work based on explicit `@unstable` or `@deprecated` attributes, if the
symbol was marked in a different way this script will still warn. If a symbols
parent was marked unstable or deprecated then the symbol is also considered to
be unstable or deprecated.
"""

from collections import defaultdict
from typing import Generator, List
import sys
import os
import argparse as ap
import itertools

try:
    import chapel.core as dyno
    from chapel import each_matching
except ModuleNotFoundError:
    print("Could not find the frontend python bindings")
    print("Install with 'python3 -m pip install -e $CHPL_HOME/tools/chapel-py'")
    exit(1)


"""
helpers
"""


def get_module(n: dyno.AstNode) -> dyno.AstNode:
    if isinstance(n, dyno.Module):
        return n
    if n is not None and (parent := n.parent()):
        return get_module(parent)
    return None


def node_or_parent_has_attribute(node: dyno.AstNode, marker: str) -> bool:
    """
    a symbol is has an attribute if it has an attribute that matches the argument, or is parent does
    """
    if (attrs := node.attribute_group()) and any(a.name() == marker for a in attrs):
        return True

    if (parent := node.parent()) and node_or_parent_has_attribute(parent, marker):
        return True

    return False


def is_deprecated(node: dyno.AstNode) -> bool:
    """a node is deprecated if it or its parent has an attribute that is deprecated"""
    return node_or_parent_has_attribute(node, "deprecated")


def is_unstable(node: dyno.AstNode) -> bool:
    """a node is unstable if it or its parent has an attribute that is unstable"""
    return node_or_parent_has_attribute(node, "unstable")


def is_nodoc(node: dyno.AstNode) -> bool:
    """
    a symbol is marked nodoc for the following reasons
    - symbol has attribute `@chpldoc.nodoc`
    - symbol name is prefixed with `chpl_` and symbol does not have `pragma "chpldoc ignore chpl prefix"`
    - symbol is private
    - symbol is a child of a node that is nodoc
    """
    if (attrs := node.attribute_group()) and any(
        a.name() == "chpldoc.nodoc" for a in attrs
    ):
        return True

    if (
        hasattr(node, "name")
        and node.name().startswith("chpl_")
        and "chpldoc ignore chpl prefix" not in node.pragmas()
    ):
        return True

    if hasattr(node, "visibility") and node.visibility() == "private":
        return True

    if (parent := node.parent()) and is_nodoc(parent):
        return True

    return False


def is_docstring_comment(c: dyno.Comment) -> bool:
    """c is a docstring if it doesn't begin with '//'"""
    return not c.text().startswith("//")


def get_node_name(node: dyno.AstNode) -> str:
    """
    get the name of a node, even when it may not have a `name()` method

    if this is a member of a aggregate type, also gets that name
    if this is a member of a module, also gets that name

    For a multidecl, gets all the names

    """

    def get_simple_node_name(node: dyno.AstNode) -> str:
        name: str = None
        if hasattr(node, "name"):
            name = node.name()
        elif isinstance(node, dyno.MultiDecl):
            name = "MultiDecl[" + ", ".join(c.name() for c in node) + "]"
        else:
            name = str(node)
        return name

    name = get_simple_node_name(node)

    if hasattr(node, "this_formal") and (this := node.this_formal()):
        aggregate_name = get_simple_node_name(this.type_expression())
        name = f"{aggregate_name}.{name}"

    if mod := get_module(node):
        module_name = mod.name()
        name = f"{module_name}.{name}"

    return name


def lookAhead(iterable, window=1):
    items, nexts = itertools.tee(iterable, 2)
    nexts = itertools.islice(nexts, window, None)
    return itertools.zip_longest(items, nexts)


"""
main code
"""


class FindUndocumentedSymbols:
    def __init__(
        self,
        ast: List[dyno.AstNode],
        ignore_deprecated: bool = False,
        ignore_unstable: bool = False,
    ):
        self.ast = ast
        self.ignore_deprecated = ignore_deprecated
        self.ignore_unstable = ignore_unstable

    # (pattern, extra check fun)
    documentable_symbol_patterns = {
        "function": (
            dyno.Function,
            lambda node, match: isinstance(
                node.parent(), (dyno.Module, dyno.AggregateDecl, dyno.Interface)
            ),
        ),
        "module": (dyno.Module, lambda node, match: node.kind() != "implicit"),
        "aggregate_decl": (
            dyno.AggregateDecl,
            lambda node, match: isinstance(
                node.parent(), (dyno.Module, dyno.AggregateDecl)
            ),
        ),
        "decl": (
            dyno.Variable,
            lambda node, match: isinstance(
                node.parent(), (dyno.Module, dyno.AggregateDecl)
            ),
        ),
        "multi_decl": (
            dyno.MultiDecl,
            lambda node, match: isinstance(
                node.parent(), (dyno.Module, dyno.AggregateDecl)
            ),
        ),
        "enum": (
            dyno.Enum,
            lambda node, match: isinstance(
                node.parent(), (dyno.Module, dyno.AggregateDecl)
            ),
        ),
        "enum_element": (dyno.EnumElement, None),
        "interface": (
            dyno.Interface,
            lambda node, match: isinstance(
                node.parent(), (dyno.Module, dyno.AggregateDecl)
            ),
        ),
    }

    def _get_previous_sibling(self, node: dyno.AstNode):
        parent = node.parent()
        if not parent:
            for sib1, sib2 in lookAhead(self.ast):
                if sib2 and node.unique_id() == sib2.unique_id():
                    return sib1
            return None

        for sib1, sib2 in lookAhead(parent):
            if sib2 and node.unique_id() == sib2.unique_id():
                return sib1
        return None

    def get_documentable_symbols(self, root: dyno.AstNode) -> Generator:
        for (
            pat,
            check_func,
        ) in FindUndocumentedSymbols.documentable_symbol_patterns.values():
            matches = [
                m for m in each_matching(root, pat) if not check_func or check_func(*m)
            ]
            for node, _ in matches:
                yield node

    def has_doc_comment(self, node: dyno.AstNode) -> bool:
        """
        This node has a doc comment if
        - the previous node is a comment
        """

        if (
            (prev := self._get_previous_sibling(node))
            and isinstance(prev, dyno.Comment)
            and is_docstring_comment(prev)
        ):
            return True

        return False

    def __call__(self):
        syms = [s for s in self.get_documentable_symbols([a for a in self.ast])]
        for s in syms:
            if not is_nodoc(s) and not self.has_doc_comment(s):
                if self.ignore_deprecated and is_deprecated(s):
                    continue
                if self.ignore_unstable and is_unstable(s):
                    continue
                yield s


def get_files(files: List[str], check_suffix=False) -> Generator:
    """
    Yield all chapel files, following directories recursively
    """

    for file in files:
        if os.path.isfile(file):
            # just yield any file given if not checking the suffix
            if not check_suffix:
                yield file
            elif check_suffix and os.path.splitext(file)[1] == ".chpl":
                yield file

        elif os.path.isdir(file):
            for root, subdirs, files in os.walk(file):
                # yield files in directory files, always check the suffix
                yield from get_files([os.path.join(root, file) for file in files], True)
                # recurse into sub dirs, always checl the suffix
                yield from get_files([os.path.join(root, d) for d in subdirs], True)
        else:
            print("Error: path was not file or directory", file=sys.stderr)
            exit(1)


def get_trees(filenames: List[str]) -> Generator:
    """
    Yield dyno asts and their filename.
    Some files might have the same name, which Dyno really doesn't like.
    Strateify files into "buckets"; within each bucket, all filenames are
    unique. Between each bucket, re-create the Dyno context to avoid giving
    it complicting files.
    """
    basenames = defaultdict(lambda: 0)
    buckets = defaultdict(lambda: [])
    for filename in filenames:
        filename_expanded = os.path.realpath(os.path.expandvars(filename))

        basename = os.path.basename(filename_expanded)
        bucket = basenames[basename]
        basenames[basename] += 1
        buckets[bucket].append((filename, filename_expanded))

    for filenames in buckets.values():
        ctx = dyno.Context()
        for filename, filename_expanded in filenames:
            ast = ctx.parse(filename_expanded)
            yield (filename, ast)


def main(raw_args: List[str]) -> int:
    a = ap.ArgumentParser()
    a.add_argument("files", nargs="*")
    a.add_argument("--ignore-deprecated", action="store_true", default=False)
    a.add_argument("--ignore-unstable", action="store_true", default=False)
    args = a.parse_args(raw_args)

    for filename, astList in get_trees(get_files(args.files)):
        fus = FindUndocumentedSymbols(
            astList,
            ignore_deprecated=args.ignore_deprecated,
            ignore_unstable=args.ignore_unstable,
        )
        for sym in fus():
            loc = sym.location()
            (line, col) = loc.start()
            name = get_node_name(sym)

            print(f"warning: '{name}' at {filename}:{line}:{col} is undocumented")

    return 0


if __name__ == "__main__":
    exit(main(sys.argv[1:]))
