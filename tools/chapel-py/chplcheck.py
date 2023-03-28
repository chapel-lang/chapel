import chapel
import re

def parse_attribute(attr, attribute):
    (name, formals) = attribute

    if attr.name() != ("chplcheck." + name): return None

    parse_result = {}
    def save_kw(actual_tuple):
        (name, value) = actual_tuple
        if name not in formals:
            raise Exception("Invalid named argument for attribute!")
        elif name in parse_result:
            raise Exception("Duplicate occurrence of named argument!")
        else:
            parse_result[name] = value

    # First, process arguments in order
    actuals = attr.actuals()
    for (actual, formal) in zip(actuals, formals):
        # If we found the first named argument, save it and stop
        # processing positional arguments.
        if isinstance(actual, tuple):
            save_kw(actual)
            break

        # Otherwise, it's a positional argument, save it under the current formal.
        parse_result[formal] = actual

    # Finish up with the remaining kwargs, if any.
    for actual in actuals:
        if not isinstance(actual, tuple): raise Exception("Mixing named and positional arguments!")
        save_kw(actual)

    for formal in formals:
        if formal not in parse_result:
            parse_result[formal] = None

    return parse_result

def preorder(node):
    yield node
    for child in node:
        yield from preorder(child)

def each(node, nodetype):
    for child in preorder(node):
        if isinstance(child, nodetype):
            yield child

def ignores_rule(node, rulename):
    ag = node.attribute_group()

    if ag is None: return False
    for attr in ag:
        attr_call = parse_attribute(attr, IgnoreAttr)
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
            print("Node {} violates rule {}".format(node, name))

IgnoreAttr = ("ignore", ["rule", "comment"])

def check_camel_case(node):
    return re.fullmatch(r'[a-z]+([A-Z][a-z]+)*|[A-Z]+', node.name())

Rules = [
    ("RequireCamelCase", chapel.NamedDecl, check_camel_case)
]

ctx = chapel.Context()
ast = ctx.parse("oneplusone.chpl")

ast.dump()

for rule in Rules:
    check_rule(ast, rule)
