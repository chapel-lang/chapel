#
# Copyright 2024-2024 Hewlett Packard Enterprise Development LP
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

from typing import Optional, Union
import chapel


def get_symbol_signature(node: chapel.AstNode, resolve: bool = False) -> str:
    """
    get a string representation of a AstNode's signature

    If resolve is True, it will attempt to resolve inferred types and values
    """
    if not isinstance(node, chapel.NamedDecl):
        return _node_to_string(node, resolve)

    if isinstance(node, chapel.Class) or isinstance(
        node, chapel.Record
    ):
        return _record_class_to_string(node)
    elif isinstance(node, chapel.Interface):
        return f"interface {node.name()}"
    elif isinstance(node, chapel.Module):
        return f"module {node.name()}"
    elif isinstance(node, chapel.Enum):
        return f"enum {node.name()}"
    elif isinstance(node, chapel.Variable):
        return _var_to_string(node, resolve)
    elif isinstance(node, chapel.Function):
        return _proc_to_string(node)

    return node.name()


def _node_to_string(node: chapel.AstNode, resolve: bool = False) -> str:
    """
    General helper method to convert an AstNode to a string representation. If
    it doesn't know how to convert the node, it returns "<...>"
    """
    if isinstance(node, chapel.NamedDecl):
        return get_symbol_signature(node, resolve)
    elif isinstance(node, chapel.Identifier):
        return node.name()
    elif isinstance(node, chapel.IntLiteral):
        return node.text()
    elif isinstance(node, chapel.UintLiteral):
        return node.text()
    elif isinstance(node, chapel.BoolLiteral):
        return node.value()
    elif isinstance(node, chapel.ImagLiteral):
        return node.text()
    elif isinstance(node, chapel.RealLiteral):
        return node.text()
    elif isinstance(node, chapel.StringLiteral):
        return '"' + node.value() + '"'
    elif isinstance(node, chapel.CStringLiteral):
        return 'c"' + node.value() + '"'
    return "<...>"


def _record_class_to_string(
    node: Union[chapel.Record, chapel.Class]
) -> str:
    """
    Convert a Record or a Class to a string
    """
    keyword = "record" if isinstance(node, chapel.Record) else "class"

    s = ""
    ie = list(node.inherit_exprs())
    if len(ie) > 0:
        s = ": " + ", ".join([_node_to_string(x) for x in ie])
    prefix = ""
    if node.linkage():
        prefix += f"{node.linkage()} "
    if node.linkage_name():
        prefix += f"{_node_to_string(node.linkage_name())} "

    return f"{prefix}{keyword} {node.name()}{s}"


def _var_to_string(node: chapel.VarLikeDecl, resolve: bool = False) -> str:
    """
    Convert a VarLikeDecl to a string
    """
    s = ""
    if node.visibility():
        s += f"{node.visibility()} "
    if node.linkage():
        s += f"{node.linkage()} "
    if node.linkage_name():
        s += f"{_node_to_string(node.linkage_name())} "

    if isinstance(node, chapel.Variable):
        if node.is_config():
            s += "config "
    intent = _intent_to_string(node.intent())
    if intent:
        s += f"{intent} "
    s += node.name()

    s += f": {get_symbol_type(node, resolve)}"
    s += f" = {get_symbol_value(node, resolve)}"

    return s


def _proc_to_string(node: chapel.Function) -> str:
    """
    Convert a function to a string
    """
    s = ""

    if node.visibility():
        s += f"{node.visibility()} "
    if node.linkage():
        s += f"{node.linkage()} "
    if node.linkage_name():
        s += f"{_node_to_string(node.linkage_name())} "
    if node.is_override():
        s += "override "
    if node.is_inline():
        s += "inline "

    s += f"{node.kind()} "
    # if it has a this-formal, check for this intent
    if node.this_formal() and _intent_to_string(node.this_formal().intent()):
        s += f"{_intent_to_string(node.this_formal().intent())} "
    s += f"{node.name()}"

    if not node.is_parenless():
        start_idx = 1 if node.this_formal() else 0
        formal_strings = [
            _var_to_string(node.formal(i))
            for i in range(start_idx, node.num_formals())
        ]
        s += f"({', '.join(formal_strings)})"

    if _intent_to_string(node.return_intent()):
        s += f" {_intent_to_string(node.return_intent())}"
    if node.return_type():
        s += f": {_node_to_string(node.return_type())}"
    if node.throws():
        s += " throws"
    if node.where_clause():
        s += f" where {_node_to_string(node.where_clause())}"

    return s


def _intent_to_string(intent: Optional[str]) -> str:
    """
    convert an intent (as reported by Dyno) to a user-facing string
    """
    remap = {
        "<default-intent>": "",
        "<index>": "",
        "<const-var>": "const",
    }
    # use 'intent' as the default, so if no remap no work done
    return remap.get(intent, intent) if intent else ""


def get_symbol_type(node: chapel.AstNode, resolve: bool = False) -> str:
    """Get the type of a symbol"""

    qt = node.type() if resolve else None
    if not qt:
        # could not resolve the type, try and interpolate the type_expression (if it has one)
        if isinstance(node, chapel.VarLikeDecl):
            type_ = node.type_expression()
            if type_:
                return _node_to_string(type_, resolve)
        # otherwise, return a placeholder text
        return "<unknown type>"
    else:
        _, type_, _ = qt
        if isinstance(type_, chapel.ErroneousType):
            return "<error>"
        return str(type_)

def get_symbol_value(node: chapel.AstNode, resolve: bool = False) -> str:
    """Get the value of a symbol"""

    qt = node.type() if resolve else None

    if not qt or qt[2] is None:
        # could not resolve the type, try and interpolate the init_expression (if it has one)
        if isinstance(node, chapel.VarLikeDecl):
            init = node.init_expression()
            if init:
                return _node_to_string(init, resolve)
        # otherwise, return a placeholder text
        return "<...>"
    else:
        _, _, param = qt
        return str(param)
