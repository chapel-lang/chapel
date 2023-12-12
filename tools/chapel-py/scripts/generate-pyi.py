#!/usr/bin/env python3
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

import chapel.core
from typing import List, Tuple


def _get_base_header() -> str:
    c = chapel.core.Context()
    return c._get_pyi_file()


def _wrap_docstring(docstring: str, indent: int = 8) -> List[str]:
    spaces = " " * indent
    triple_quote = spaces + '"""'
    return [triple_quote, spaces + docstring, triple_quote]


def _def_body(indent: int = 8) -> List[str]:
    spaces = " " * indent
    return [spaces + "..."]


def _wrap_method(
    name: str,
    args: List[Tuple[str, str | None]] = [],
    rettype: str = "None",
    indent: int = 4,
) -> List[str]:
    spaces = " " * indent
    argstr = ""
    for a, t in args:
        typestr = f": {t}" if t else ""
        argstr += f", {a}{typestr}"
    return [spaces + f"def {name}(self{argstr}) -> {rettype}:"]


def get_Context_header() -> str:
    s = [
        "class Context:",
        *_wrap_method("parse", args=[("path", "str")], rettype="List[AstNode]"),
        *_wrap_docstring("Parse a top-level AST node from the given file"),
        *_def_body(),
        "",
        *_wrap_method("is_bundled_path", args=[("path", "str")], rettype="bool"),
        *_wrap_docstring(
            "Check if the given file path is within the bundled (built-in) Chapel files"
        ),
        *_def_body(),
        "",
        *_wrap_method("advance_to_next_revision"),
        *_wrap_docstring("Advance the context to the next revision"),
        *_def_body(),
        "",
        *_wrap_method("track_errors", rettype="ErrorManager"),
        *_wrap_docstring("Generate a stub file for the Chapel AST nodes"),
        *_def_body(),
        "",
        *_wrap_method("_get_pyi_file", rettype="str"),
        *_wrap_docstring("Generate a basic pyi stub file"),
        *_def_body(),
        "",
    ]
    return "\n".join(s)


def get_Location_header() -> str:
    s = [
        "class Location:",
        *_wrap_method("start", rettype="int"),
        *_wrap_docstring("Get the start of a Location object"),
        *_def_body(),
        "",
        *_wrap_method("end", rettype="int"),
        *_wrap_docstring("Get the end of a Location object"),
        *_def_body(),
        "",
        *_wrap_method("path", rettype="str"),
        *_wrap_docstring("Get the path of a Location object"),
        *_def_body(),
        "",
    ]
    return "\n".join(s)


def get_ErrorManager_header() -> str:
    s = [
        "class ErrorManager:",
        *_wrap_method("__enter__", rettype="List[Error]"),
        *_def_body(),
        "",
        *_wrap_method("__exit__", args=[("*args", None)]),
        *_def_body(),
        "",
    ]
    return "\n".join(s)


def get_Error_header() -> str:
    s = [
        "class Error:",
        *_wrap_method("location", rettype="Location"),
        *_wrap_docstring("Get the location at which this error occurred"),
        *_def_body(),
        "",
        *_wrap_method("message", rettype="str"),
        *_wrap_docstring("Retrieve the contents of this error message"),
        *_def_body(),
        "",
        *_wrap_method("kind", rettype="str"),
        *_wrap_docstring(
            "Retrieve the kind ('error', 'warning') of this type of error"
        ),
        *_def_body(),
        "",
        *_wrap_method("type", rettype="str"),
        *_wrap_docstring("Retrieve the unique name of this type of error"),
        *_def_body(),
        "",
    ]
    return "\n".join(s)


def get_AstNode_header() -> str:
    s = [
        "class AstNode:",
        *_wrap_method("dump"),
        *_wrap_docstring("Dump the internal representation of the given AST node"),
        *_def_body(),
        "",
        *_wrap_method("tag", rettype="str"),
        *_wrap_docstring("Get a string representation of the AST node's type"),
        *_def_body(),
        "",
        *_wrap_method("attribute_group", rettype="AttributeGroup | None"),
        *_wrap_docstring("Get the attribute group, if any, associated with this node"),
        *_def_body(),
        "",
        *_wrap_method("location", rettype="Location"),
        *_wrap_docstring("Get the location of this AST node in its file"),
        *_def_body(),
        "",
        *_wrap_method("parent", rettype="AstNode | None"),
        *_wrap_docstring("Get the parent node of this AST node"),
        *_def_body(),
        "",
        *_wrap_method("pragmas", rettype="Set[str]"),
        *_wrap_docstring("Get the pragmas of this AST node"),
        *_def_body(),
        "",
        *_wrap_method("unique_id", rettype="int"),
        *_wrap_docstring("Get a unique identifier for this AST node"),
        *_def_body(),
        "",
        *_wrap_method("__iter__", rettype="Iterator[AstNode]"),
        *_wrap_docstring("Iterate over this AST node's children"),
        *_def_body(),
        "",
    ]
    return "\n".join(s)


replacements = {
    "class Context: pass": get_Context_header,
    "class Location: pass": get_Location_header,
    "class ErrorManager: pass": get_ErrorManager_header,
    "class Error: pass": get_Error_header,
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
