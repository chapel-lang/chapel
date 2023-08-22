import chapel
import chapel.core
import re
import sys
import argparse

def consecutive_decls(node):
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
                    yield consecutive
                consecutive = []

            # If this could be a compatible decl, start a new list.
            if new_kind is not None:
                consecutive.append(child)

        if len(consecutive) > 1:
            yield consecutive

    yield from recurse(node)

def each(node, pattern):
    for child in chapel.preorder(node):
        if chapel.match_pattern(child, pattern) is not None:
            yield child

def ignores_rule(node, rulename):
    ag = node.attribute_group()

    if ag is None: return False
    for attr in ag:
        attr_call = chapel.parse_attribute(attr, IgnoreAttr)
        if attr_call is None: continue

        ignored_rule = attr_call["rule"]
        if ignored_rule is not None and ignored_rule.value() == rulename:
            return True

    return False

def report_violation(node, name):
    if name in args.ignored_rules:
        return

    location = node.location()
    first_line, _ = location.start()
    print("{}:{}: node violates rule {}".format(location.path(), first_line, name))

def check_basic_rule(root, rule):
    (name, nodetype, func) = rule
    for node in each(root, nodetype):
        if ignores_rule(node, name): continue

        if not func(node): report_violation(node, name)

IgnoreAttr = ("chplcheck.ignore", ["rule", "comment"])

def check_nested_coforall(node):
    parent = node.parent()
    while parent is not None:
        if isinstance(parent, chapel.core.Coforall):
            return False
        parent = parent.parent()
    return True

def name_for_linting(node):
    name = node.name()
    if name.startswith("chpl_"):
        name = name.removeprefix("chpl_")
    return name

def check_camel_case(node):
    return re.fullmatch(r'_?([a-z]+([A-Z][a-z]+|\d+)*|[A-Z]+)\$?', name_for_linting(node))

def check_camel_case_var(node):
    if node.name() == "_": return True
    return check_camel_case(node)

def check_pascal_case(node):
    return re.fullmatch(r'_?(([A-Z][a-z]+|\d+)+|[A-Z]+)\$?', name_for_linting(node))

def check_reserved_prefix(node):
    if node.name().startswith("chpl_"):
        path = node.location().path()
        return ctx.is_bundled_path(path)
    return True

def check_redundant_block(node):
    return node.block_style() != "unnecessary"

Rules = [
    ("CamelCaseVariables", chapel.core.VarLikeDecl, check_camel_case_var),
    ("CamelCaseRecords", chapel.core.Record, check_camel_case),
    ("PascalCaseClasses", chapel.core.Class, check_pascal_case),
    ("PascalCaseModules", chapel.core.Module, check_pascal_case),
    ("DoKeywordAndBlock", chapel.core.Loop, check_redundant_block),
    ("NestedCoforalls", chapel.core.Coforall, check_nested_coforall),
    ("BoolLitInCondStmt", [chapel.core.Conditional, chapel.core.BoolLiteral, chapel.rest], lambda node: False),
    ("ChplPrefixReserved", chapel.core.NamedDecl, check_reserved_prefix),
]

parser = argparse.ArgumentParser( prog='chplcheck', description='A linter for the Chapel language')
parser.add_argument('filename')
parser.add_argument('--ignore-rule', action='append', dest='ignored_rules', default=[])
args = parser.parse_args()

ctx = chapel.core.Context()
ast = ctx.parse(args.filename)

for rule in Rules:
    check_basic_rule(ast, rule)

for group in consecutive_decls(ast):
    report_violation(group[1], "ConsecutiveDecls")
