import chapel
import chapel.core
import re
from driver import basic_rule, advanced_rule

def name_for_linting(node):
    name = node.name()
    if name.startswith("chpl_"):
        name = name.removeprefix("chpl_")
    return name

def check_camel_case(node):
    return re.fullmatch(r'_?([a-z]+([A-Z][a-z]+|\d+)*|[A-Z]+)\$?', name_for_linting(node))

def check_pascal_case(node):
    return re.fullmatch(r'_?(([A-Z][a-z]+|\d+)+|[A-Z]+)\$?', name_for_linting(node))

@basic_rule(chapel.core.VarLikeDecl)
def CamelCaseVariables(node):
    if node.name() == "_": return True
    return check_camel_case(node)

@basic_rule(chapel.core.Record)
def CamelCaseRecords(node):
    return check_camel_case(node)

@basic_rule(chapel.core.Class)
def PascalCaseClasses(node):
    return check_pascal_case(node)

@basic_rule(chapel.core.Module)
def PascalCaseModules(node):
    return check_pascal_case(node)

@basic_rule(chapel.core.Loop)
def DoKeywordAndBlock(node):
    return node.block_style() != "unnecessary"

@basic_rule(chapel.core.Coforall)
def NestedCoforalls(node):
    parent = node.parent()
    while parent is not None:
        if isinstance(parent, chapel.core.Coforall):
            return False
        parent = parent.parent()
    return True

@basic_rule([chapel.core.Conditional, chapel.core.BoolLiteral, chapel.rest])
def BoolLitInCondStmt(node):
    return False

@basic_rule(chapel.core.NamedDecl)
def ChplPrefixReserved(node):
    if node.name().startswith("chpl_"):
        path = node.location().path()
        return ctx.is_bundled_path(path)
    return True

@advanced_rule
def ConsecutiveDecls(root):
    def is_relevant_decl(node):
        if isinstance(node, chapel.core.MultiDecl):
            for child in node:
                if isinstance(child, chapel.core.Variable): return child.kind()
        elif isinstance(node, chapel.core.Variable):
            return node.kind()

        return None

    def recurse(node, skip_direct = False):
        consecutive = []
        last_kind = None
        last_has_attribute = False

        for child in node:
            yield from recurse(child, skip_direct = isinstance(child, chapel.core.MultiDecl))

            if skip_direct: continue

            new_kind = is_relevant_decl(child)
            has_attribute = child.attribute_group() is not None
            any_has_attribute = last_has_attribute or has_attribute
            compatible_kinds = not any_has_attribute and (last_kind is None or last_kind == new_kind)
            last_kind = new_kind
            last_has_attribute = has_attribute

            # If we ran out of compatible decls, see if we can return them.
            if not compatible_kinds:
                if len(consecutive) > 1:
                    yield consecutive[1]
                consecutive = []

            # If this could be a compatible decl, start a new list.
            if new_kind is not None:
                consecutive.append(child)

        if len(consecutive) > 1:
            yield consecutive[1]

    yield from recurse(root)

@advanced_rule
def MisleadingIndentation(root):
    prev = None
    for child in root:
        yield from MisleadingIndentation(child)

        if prev is not None:
            if child.location().start()[1] == prev.location().start()[1]:
                yield child

        if isinstance(child, chapel.core.Loop) and child.block_style() == "implicit":
            grandchildren = list(child)
            if len(grandchildren) > 0:
                prev = list(grandchildren[-1])[0]
