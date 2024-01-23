#!/usr/bin/env python3
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

import chapel.core
from typing import List, Tuple, Optional, Union


def _get_base_header() -> str:
    c = chapel.core._Context()
    return c._get_pyi_file()


def _section(*args: Union[str, List[str]], indent: int = 4) -> List[str]:
    temp = []
    for arg in args:
        if not isinstance(arg, list):
            temp.append(arg)
        else:
            temp.extend(arg)
    return [(" " * indent + s).rstrip() for s in temp]


def _wrap_docstring(docstring: str) -> List[str]:
    return ['"""', docstring, '"""']


def _wrap_method(
    name: str,
    args: List[Tuple[str, Optional[str]]] = [],
    rettype: str = "None",
    docstring: Optional[str] = None,
) -> List[str]:
    argstr = ""
    for a, t in args:
        typestr = f": {t}" if t else ""
        argstr += f", {a}{typestr}"
    methodbody = []
    if docstring:
        methodbody.extend(_wrap_docstring(docstring))
    methodbody.append("...")
    methodbody.append("")
    return _section(f"def {name}(self{argstr}) -> {rettype}:", _section(methodbody))


def get_Context_header() -> str:
    s = _section(
        "class _Context:",
        _wrap_method(
            "parse",
            args=[("path", "str")],
            rettype="typing.List[AstNode]",
            docstring="Parse a top-level AST node from the given file",
        ),
        _wrap_method(
            "is_bundled_path",
            args=[("path", "str")],
            rettype="bool",
            docstring="Check if the given file path is within the bundled (built-in) Chapel files",
        ),
        _wrap_method(
            "advance_to_next_revision",
            args=[("gc", "bool")],
            docstring="Advance the context to the next revision",
        ),
        _wrap_method(
            "track_errors",
            rettype="ErrorManager",
            docstring="Return a context manager that tracks errors emitted by this Context",
        ),
        _wrap_method(
            "_get_pyi_file",
            rettype="str",
            docstring="Generate a basic pyi stub file",
        ),
        indent=0,
    )
    return "\n".join(s)


def get_Location_header() -> str:
    s = _section(
        "class Location:",
        _wrap_method(
            "start", rettype="typing.Tuple[int, int]", docstring="Get the start of a Location object"
        ),
        _wrap_method(
            "end", rettype="typing.Tuple[int, int]", docstring="Get the end of a Location object"
        ),
        _wrap_method(
            "path", rettype="str", docstring="Get the path of a Location object"
        ),
        indent=0,
    )
    return "\n".join(s)


def get_ErrorManager_header() -> str:
    s = _section(
        "class ErrorManager:",
        _wrap_method("__enter__", rettype="typing.List[Error]"),
        _wrap_method("__exit__", args=[("*args", None)]),
        indent=0,
    )
    return "\n".join(s)


def get_Error_header() -> str:
    s = _section(
        "class Error:",
        _wrap_method(
            "location",
            rettype="Location",
            docstring="Get the location at which this error occurred",
        ),
        _wrap_method(
            "message",
            rettype="str",
            docstring="Retrieve the contents of this error message",
        ),
        _wrap_method(
            "kind",
            rettype="str",
            docstring="Retrieve the kind ('error', 'warning') of this type of error",
        ),
        _wrap_method(
            "type",
            rettype="str",
            docstring="Retrieve the unique name of this type of error",
        ),
        indent=0,
    )
    return "\n".join(s)


def get_Scope_header() -> str:
    s = _section(
        "class Scope:",
        _wrap_method("__init__", args=[("context", "Context")]),
        _wrap_method("used_imported_modules", rettype="typing.List[AstNode]", docstring="Get the modules that were used or imported in this scope"),
        indent=0,
    )
    return "\n".join(s)

def get_AstNode_header() -> str:
    s = _section(
        "class AstNode:",
        _wrap_method(
            "dump",
            docstring="Dump the internal representation of the given AST node",
        ),
        _wrap_method(
            "tag",
            rettype="str",
            docstring="Get a string representation of the AST node's type",
        ),
        _wrap_method(
            "attribute_group",
            rettype="typing.Optional[AttributeGroup]",
            docstring="Get the attribute group, if any, associated with this node",
        ),
        _wrap_method(
            "location",
            rettype="Location",
            docstring="Get the location of this AST node in its file",
        ),
        _wrap_method(
            "scope",
            rettype="typing.Optional[Scope]",
            docstring="Get the scope for this AST node, if it has one",
        ),
        _wrap_method(
            "parent",
            rettype="typing.Optional[AstNode]",
            docstring="Get the parent node of this AST node",
        ),
        _wrap_method(
            "pragmas",
            rettype="typing.Set[str]",
            docstring="Get the pragmas of this AST node",
        ),
        _wrap_method(
            "unique_id",
            rettype="str",
            docstring="Get a unique identifier for this AST node",
        ),
        _wrap_method(
            "__iter__",
            rettype="typing.Iterator[AstNode]",
            docstring="Iterate over this AST node's children",
        ),
        indent=0,
    )
    return "\n".join(s)


replacements = {
    "class _Context: pass": get_Context_header,
    "class Location: pass": get_Location_header,
    "class ErrorManager: pass": get_ErrorManager_header,
    "class Error: pass": get_Error_header,
    "class Scope: pass": get_Scope_header,
    "class AstNode: pass": get_AstNode_header,
}


def main():
    pyi = _get_base_header()

    for k, text_func in replacements.items():
        pyi = pyi.replace(k, text_func())

    print(pyi)

    return 0


if __name__ == "__main__":
    exit(main())
