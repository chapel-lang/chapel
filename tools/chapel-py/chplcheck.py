import chapel
import chapel.core
import re

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

def check_rule(root, rule):
    (name, nodetype, func) = rule
    for node in each(root, nodetype):
        if ignores_rule(node, name): continue

        if not func(node):
            location = node.location()
            first_line, _ = location.start()
            print("{}:{}: node violates rule {}".format(location.path(), first_line, name))

IgnoreAttr = ("chplcheck.ignore", ["rule", "comment"])

def check_camel_case(node):
    return re.fullmatch(r'[a-z]+([A-Z][a-z]+)*|[A-Z]+', node.name())

Rules = [
    ("RequireCamelCase", chapel.core.NamedDecl, check_camel_case)
]

ctx = chapel.core.Context()
ast = ctx.parse("oneplusone.chpl")

for rule in Rules:
    check_rule(ast, rule)
