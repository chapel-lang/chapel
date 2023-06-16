import chapel
import chapel.core
import re

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

        for child in node:
            yield from recurse(child, skip_direct = isinstance(child, chapel.core.MultiDecl))

            if skip_direct: continue

            new_kind = is_relevant_decl(child)
            compatible_kinds = (last_kind is None or last_kind == new_kind)
            last_kind = new_kind

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
    location = node.location()
    first_line, _ = location.start()
    print("{}:{}: node violates rule {}".format(location.path(), first_line, name))

def check_rule(root, rule):
    (name, nodetype, func) = rule
    for node in each(root, nodetype):
        if ignores_rule(node, name): continue

        if not func(node): report_violation(node, name)

IgnoreAttr = ("chplcheck.ignore", ["rule", "comment"])

def check_camel_case(node):
    return re.fullmatch(r'[a-z]+([A-Z][a-z]+)*|[A-Z]+', node.name())

def check_redundant_block(node):
    return node.block_style() != "unnecessary"

Rules = [
    ("RequireCamelCase", chapel.core.NamedDecl, check_camel_case),
    ("KeywordAndBlock", chapel.core.Loop, check_redundant_block)
]

ctx = chapel.core.Context()
ast = ctx.parse("demo/oneplusone.chpl")

for rule in Rules:
    check_rule(ast, rule)

for group in consecutive_decls(ast):
    report_violation(group[1], "ConsecutiveDecls")
