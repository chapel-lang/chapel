#
# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
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

    def silence_rules(self, *rules):
        self.SilencedRules.extend(rules)

    def should_check_rule(self, node, rulename):
        if rulename in self.SilencedRules:
            return False

        if node is not None and ignores_rule(node, rulename):
            return False

        return True

    def check_basic_rule(self, context, root, rule):
        (name, nodetype, func) = rule

        if not self.should_check_rule(None, name):
            return

        # If we should ignore the rule no matter the node, no reason to run
        # a traversal and match the pattern.
        for (node, _) in chapel.each_matching(root, nodetype):
            if not self.should_check_rule(node, name):
                continue

            if not func(context, node):
                yield (node, name)

    def check_advanced_rule(self, context, root, rule):
        (name, func) = rule

        # If we should ignore the rule no matter the node, no reason to run
        # a traversal and match the pattern.
        if not self.should_check_rule(None, name):
            return

        for node in func(context, root):
            yield (node, name)

    def basic_rule(self, nodetype):
        def wrapper(func):
            self.BasicRules.append((func.__name__, nodetype, func))
            return func
        return wrapper

    def advanced_rule(self, func):
        self.AdvancedRules.append((func.__name__, func))
        return func

    def run_checks(self, context, asts):
        for ast in asts:
            for rule in self.BasicRules:
                yield from self.check_basic_rule(context, ast, rule)

            for rule in self.AdvancedRules:
                yield from self.check_advanced_rule(context, ast, rule)
