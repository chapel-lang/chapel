from typing import Optional, Union
import chapel


# TODO: for now, just text based. but should resolve generic types
def get_symbol_signature(node: chapel.core.AstNode) -> str:
    """
    get a string representation of a AstNode's signature

    Note: this is purely textual and does not resolve generic types
    """
    if not isinstance(node, chapel.core.NamedDecl):
        return _node_to_string(node)

    if isinstance(node, chapel.core.Class) or isinstance(
        node, chapel.core.Record
    ):
        return _record_class_to_string(node)
    elif isinstance(node, chapel.core.Interface):
        return f"interface {node.name()}"
    elif isinstance(node, chapel.core.Module):
        return f"module {node.name()}"
    elif isinstance(node, chapel.core.Enum):
        return f"enum {node.name()}"
    elif isinstance(node, chapel.core.Variable):
        return _var_to_string(node)
    elif isinstance(node, chapel.core.Function):
        return _proc_to_string(node)

    return node.name()


def _node_to_string(node: chapel.core.AstNode) -> str:
    """
    General helper method to convert an AstNode to a string representation. If
    it doesn't know how to convert the node, it returns "<...>"
    """
    if isinstance(node, chapel.core.NamedDecl):
        return get_symbol_signature(node)
    elif isinstance(node, chapel.core.Identifier):
        return node.name()
    elif isinstance(node, chapel.core.IntLiteral):
        return node.text()
    elif isinstance(node, chapel.core.UintLiteral):
        return node.text()
    elif isinstance(node, chapel.core.BoolLiteral):
        return node.value()
    elif isinstance(node, chapel.core.ImagLiteral):
        return node.text()
    elif isinstance(node, chapel.core.RealLiteral):
        return node.text()
    elif isinstance(node, chapel.core.StringLiteral):
        return '"' + node.value() + '"'
    elif isinstance(node, chapel.core.CStringLiteral):
        return 'c"' + node.value() + '"'
    return "<...>"


def _record_class_to_string(
    node: Union[chapel.core.Record, chapel.core.Class]
) -> str:
    """
    Convert a Record or a Class to a string
    """
    keyword = "record" if isinstance(node, chapel.core.Record) else "class"

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


def _var_to_string(node: chapel.core.VarLikeDecl) -> str:
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

    if isinstance(node, chapel.core.Variable):
        if node.is_config():
            s += "config "
    intent = _intent_to_string(node.intent())
    if intent:
        s += f"{intent} "
    s += node.name()
    type_ = node.type_expression()
    if type_:
        s += f": {_node_to_string(type_)}"
    init = node.init_expression()
    if init:
        s += f" = {_node_to_string(init)}"
    return s


def _proc_to_string(node: chapel.core.Function) -> str:
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
