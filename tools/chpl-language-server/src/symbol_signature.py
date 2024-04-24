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

from typing import List, Optional, Union
import chapel
from dataclasses import dataclass
import enum


class ComponentTag(enum.Enum):
    STRING = enum.auto()
    TYPE = enum.auto()
    PARAM_VALUE = enum.auto()
    PLACEHOLDER = enum.auto()


@dataclass
class Component:
    tag: ComponentTag
    value: Union[str, None]
    node: Optional[chapel.AstNode] = None

    @staticmethod
    def to_string(comps: List["Component"]) -> str:
        s = ""
        for comp in comps:
            if comp.tag in (
                ComponentTag.STRING,
                ComponentTag.TYPE,
                ComponentTag.PARAM_VALUE,
            ):
                if comp.value is not None:
                    s += str(comp.value)
            elif comp.tag == ComponentTag.PLACEHOLDER:
                s += "<...>"
            else:
                assert False, f"unknown component tag {comp.tag}"
        return s


def _wrap_str(s: str) -> Component:
    return Component(ComponentTag.STRING, s)


class SymbolSignature:
    def __init__(self, node: chapel.AstNode):
        self.node = node
        self._signature = _get_symbol_signature(self.node)

    def compute_type(self, via: Optional[chapel.TypedSignature] = None):
        """
        if types aren't textually present, try to resolve them

        Note: this is a temporary method that will go away when the resolver is fully online
        """
        for i in range(len(self._signature)):
            tag = self._signature[i].tag
            node = self._signature[i].node
            if tag == ComponentTag.TYPE and node is not None:
                type_str = _resolve_type_str(node, via)
                if type_str:
                    self._signature[i] = _wrap_str(": " + type_str)

    def compute_value(self, via: Optional[chapel.TypedSignature] = None):
        """evaluate expressions"""
        for i in range(len(self._signature)):
            tag = self._signature[i].tag
            node = self._signature[i].node
            if tag == ComponentTag.PARAM_VALUE and node is not None:
                param_str = _resolve_param_str(node, via)
                if param_str:
                    self._signature[i] = _wrap_str(" = " + param_str)

    def __str__(self) -> str:
        return Component.to_string(self._signature)


def _resolve_type_str(
    node: chapel.AstNode, via: Optional[chapel.TypedSignature] = None
) -> Optional[str]:
    """Resolve a type expression to a string"""
    rr = node.resolve_via(via) if via else node.resolve()
    if not rr:
        return None

    qt = rr.type()
    if not qt:
        return None

    _, type_, _ = qt
    if isinstance(type_, chapel.ErroneousType):
        return None
    return str(type_)


def _resolve_param_str(
    node: chapel.AstNode, via: Optional[chapel.TypedSignature] = None
) -> Optional[str]:
    """Resolve a type expression to a string"""
    rr = node.resolve_via(via) if via else node.resolve()
    if not rr:
        return None

    qt = rr.type()
    if not qt:
        return None
    _, _, param = qt
    if param is None:
        return None
    return str(param)


def _get_symbol_signature(node: chapel.AstNode) -> List[Component]:
    if not isinstance(node, chapel.NamedDecl):
        return _node_to_string(node)

    if isinstance(node, chapel.Class) or isinstance(node, chapel.Record):
        return _record_class_to_string(node)
    elif isinstance(node, chapel.Interface):
        return [_wrap_str(f"interface {node.name()}")]
    elif isinstance(node, chapel.Module):
        return [_wrap_str(f"module {node.name()}")]
    elif isinstance(node, chapel.Enum):
        return [_wrap_str(f"enum {node.name()}")]
    elif isinstance(node, chapel.VarLikeDecl):
        return _var_to_string(node)
    elif isinstance(node, chapel.Function):
        return _proc_to_string(node)
    elif isinstance(node, chapel.TypeQuery):
        return [_wrap_str(f"?{node.name()}")]

    return [_wrap_str(node.name())]


def _node_to_string(node: chapel.AstNode) -> List[Component]:
    """
    General helper method to convert an AstNode to a string representation. If
    it doesn't know how to convert the node, it returns "<...>"
    """
    if isinstance(node, chapel.NamedDecl):
        return _get_symbol_signature(node)
    elif isinstance(node, chapel.Identifier):
        return [_wrap_str(node.name())]
    elif isinstance(node, chapel.BoolLiteral):
        return [_wrap_str("true" if node.value() else "false")]
    elif isinstance(
        node,
        (
            chapel.IntLiteral,
            chapel.UintLiteral,
            chapel.ImagLiteral,
            chapel.RealLiteral,
        ),
    ):
        return [_wrap_str(node.text())]
    elif isinstance(node, chapel.StringLiteral):
        return [_wrap_str('"' + node.value() + '"')]
    elif isinstance(node, chapel.CStringLiteral):
        return [_wrap_str('c"' + node.value() + '"')]
    elif isinstance(node, chapel.FnCall):
        return _fncall_to_string(node)
    return [Component(ComponentTag.PLACEHOLDER, None)]


def _record_class_to_string(
    node: Union[chapel.Record, chapel.Class]
) -> List[Component]:
    """
    Convert a Record or a Class to a string
    """
    comps = []
    if node.linkage():
        comps.append(_wrap_str(f"{node.linkage()} "))
    if node.linkage_name():
        comps.extend(_node_to_string(node.linkage_name()))

    keyword = "record" if isinstance(node, chapel.Record) else "class"
    comps.append(_wrap_str(f"{keyword} {node.name()}"))

    ie = list(node.inherit_exprs())
    if len(ie) > 0:
        comps.append(_wrap_str(": "))
        do_comma = False
        for x in ie:
            if do_comma:
                comps.append(_wrap_str(", "))
            do_comma = True
            comps.extend(_node_to_string(x))

    return comps


def _var_to_string(node: chapel.VarLikeDecl) -> List[Component]:
    """
    Convert a VarLikeDecl to a string
    """
    comps = []

    if node.visibility():
        comps.append(_wrap_str(f"{node.visibility()} "))
    if node.linkage():
        comps.append(_wrap_str(f"{node.linkage()} "))
    if node.linkage_name():
        comps.extend(_node_to_string(node.linkage_name()))

    if isinstance(node, chapel.Variable):
        if node.is_config():
            comps.append(_wrap_str("config "))
    intent = _intent_to_string(node.intent())
    if intent:
        comps.append(_wrap_str(f"{intent} "))

    comps.append(_wrap_str(node.name()))

    type_str = None
    type_ = node.type_expression()
    if type_:
        type_str = ": " + Component.to_string(_node_to_string(type_))
    comps.append(Component(ComponentTag.TYPE, type_str, node))

    init_str = None
    init = node.init_expression()
    if init:
        init_str = " = " + Component.to_string(_node_to_string(init))
    comps.append(Component(ComponentTag.PARAM_VALUE, init_str, node))

    return comps


def _proc_to_string(node: chapel.Function) -> List[Component]:
    """
    Convert a function to a string
    """
    comps = []

    if node.visibility():
        comps.append(_wrap_str(f"{node.visibility()} "))
    if node.linkage():
        comps.append(_wrap_str(f"{node.linkage()} "))
    if node.linkage_name():
        comps.extend(_node_to_string(node.linkage_name()))
    if node.is_override():
        comps.append(_wrap_str("override "))
    if node.is_inline():
        comps.append(_wrap_str("inline "))

    comps.append(_wrap_str(f"{node.kind()} "))
    # if it has a this-formal, check for this intent
    if node.this_formal() and _intent_to_string(node.this_formal().intent()):
        comps.append(
            _wrap_str(f"{_intent_to_string(node.this_formal().intent())} ")
        )
    comps.append(_wrap_str(node.name()))

    if not node.is_parenless():
        start_idx = 1 if node.this_formal() else 0
        comps.append(_wrap_str("("))
        do_comma = False
        for i in range(start_idx, node.num_formals()):
            if do_comma:
                comps.append(_wrap_str(", "))
            do_comma = True
            comps.extend(_node_to_string(node.formal(i)))
        comps.append(_wrap_str(")"))

    if _intent_to_string(node.return_intent()):
        comps.append(_wrap_str(f" {_intent_to_string(node.return_intent())}"))
    if node.return_type():
        comps.append(_wrap_str(f": "))
        comps.extend(_node_to_string(node.return_type()))
    if node.throws():
        comps.append(_wrap_str(" throws"))
    if node.where_clause():
        comps.append(_wrap_str(" where "))
        comps.extend(_node_to_string(node.where_clause()))

    return comps


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


def _fncall_to_string(call: chapel.FnCall) -> List[Component]:
    """
    Convert a call to a string
    """
    comps = []

    comps.extend(_node_to_string(call.called_expression()))
    openbr, closebr = ("[", "]") if call.used_square_brackets() else ("(", ")")
    comps.append(_wrap_str(openbr))
    sep = ""
    for a in call.actuals():
        comps.append(_wrap_str(sep))
        sep = ", "
        if isinstance(a, tuple):
            comps.append(_wrap_str(a[0]))
            comps.append(_wrap_str(" = "))
            comps.extend(_node_to_string(a[1]))
        else:
            assert isinstance(a, chapel.AstNode)
            comps.extend(_node_to_string(a))
    comps.append(_wrap_str(closebr))

    return comps
