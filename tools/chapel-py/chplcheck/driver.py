import chapel
import chapel.core

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

class LintDriver:
    def __init__(self):
        self.SilencedRules = []
        self.BasicRules = []
        self.AdvancedRules = []

    def should_check_rule(self, node, rulename):
        if rulename in self.SilencedRules:
            return False

        if node is not None and ignores_rule(node, rulename):
            return False

        return True

    def check_basic_rule(self, root, rule):
        (name, nodetype, func) = rule

        if not self.should_check_rule(None, name):
            return

        # If we should ignore the rule no matter the node, no reason to run
        # a traversal and match the pattern.
        for (node, _) in chapel.each_matching(root, nodetype):
            if not self.should_check_rule(node, name):
                continue

            if not func(node):
                yield (node, name)

    def check_advanced_rule(self, root, rule):
        (name, func) = rule

        # If we should ignore the rule no matter the node, no reason to run
        # a traversal and match the pattern.
        if not self.should_check_rule(None, name):
            return

        for node in func(root):
            yield (node, name)

    def basic_rule(self, nodetype):
        def wrapper(func):
            self.BasicRules.append((func.__name__, nodetype, func))
            return func
        return wrapper

    def advanced_rule(self, func):
        self.AdvancedRules.append((func.__name__, func))
        return func

    def run_checks(self, asts):
        for ast in asts:
            for rule in self.BasicRules:
                yield from self.check_basic_rule(ast, rule)

            for rule in self.AdvancedRules:
                yield from self.check_advanced_rule(ast, rule)
