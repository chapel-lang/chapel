import chapel
import chapel.core

SilencedRules = []
BasicRules = []
AdvancedRules = []

IgnoreAttr = ("chplcheck.ignore", ["rule", "comment"])
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

def should_check_rule(node, rulename):
    if rulename in SilencedRules:
        return False

    if node is not None and ignores_rule(node, rulename):
        return False

    return True

def print_violation(node, name):
    location = node.location()
    first_line, _ = location.start()
    print("{}:{}: node violates rule {}".format(location.path(), first_line, name))

def check_basic_rule(root, rule):
    # If we should ignore the rule no matter the node, no reason to run
    # a traversal and match the pattern.
    if not should_check_rule(None, rule):
        return

    (name, nodetype, func) = rule
    for (node, _) in chapel.each_matching(root, nodetype):
        if not should_check_rule(node, name):
            continue

        if not func(node):
            yield (node, name)

def check_advanced_rule(root, rule):
    # If we should ignore the rule no matter the node, no reason to run
    # a traversal and match the pattern.
    if not should_check_rule(None, rule):
        return

    (name, func) = rule
    for node in func(root):
        yield (node, name)

def basic_rule(nodetype):
    def wrapper(func):
        BasicRules.append((func.__name__, nodetype, func))
        return func
    return wrapper

def advanced_rule(func):
    AdvancedRules.append((func.__name__, func))
    return func

