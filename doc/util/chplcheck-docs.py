"""
Auto-generates a *.rst for the chplcheck rules defined
`$CHPL_HOME/tools/chplcheck/src/rules.py`
"""

import sys
import os
import typing
from dataclasses import dataclass
import argparse as ap
import ast
import shutil
import chpl2rst


@dataclass
class Rule:
    name: str
    description: str
    patterns: typing.List[str]
    default: bool
    settings: typing.List[str]
    example_text: str = ""

    def rst(self):
        lines = []
        lines.append(self.name)
        lines.append("~" * len(self.name))
        lines.append("")
        lines.append("Is enabled by default? " + ("Yes" if self.default else "No"))
        lines.append("")
        lines.append(self.description)
        lines.append("")
        if self.example_text:
            lines.append(self.example_text)
            lines.append("")

        return "\n".join(lines)

    def add_example(self, example_directory: str):
        # find the example file
        example_file = os.path.join(example_directory, self.name + ".chpl")
        if not os.path.exists(example_file):
            return

        with open(example_file) as handle:
            pieces = chpl2rst.to_pieces(handle, False)
            rstoutput = chpl2rst.gen_rst(pieces, example_file)
            print(rstoutput)

        self.example_text = rstoutput



def find_rules(file: str):

    def get_rule(func) -> typing.Optional[Rule]:
        """
        Given a function definition, return a Rule object if it is a rule, otherwise None
        """
        if not isinstance(func, ast.FunctionDef):
            return None

        # find all decorators by walking decorator_list and filtering for Attribute
        decorators = [
            i
            for d in func.decorator_list
            for i in ast.walk(d)
            if isinstance(i, ast.Attribute)
        ]

        # if there is at least 1 decorator thats a `basic_rule`, an `advanced_rule`,
        # or a `location_rule`, keep it
        if not any(
            d.attr in ["basic_rule", "advanced_rule", "location_rule"]
            for d in decorators
        ):
            return None

        # get the name of the rule
        name = func.name
        # get the docstring of the rule
        description = ast.get_docstring(func) or ""

        # get the patterns, if they exist
        # the patterns are the first argument of the decorator
        # we get the string representation of the pattern
        patterns = [
            ast.unparse(d.args[0])
            for d in func.decorator_list
            if isinstance(d, ast.Call)
            and isinstance(d.func, ast.Attribute)
            and len(d.args) > 0
        ]

        # determine if the rule is enabled by default
        # default = any(d.attr == 'default' for d in decorators)
        default_settings = [
            ast.unparse(next(k.value for k in d.keywords))
            for d in func.decorator_list
            if isinstance(d, ast.Call)
            and isinstance(d.func, ast.Attribute)
            and len(d.keywords) > 0
            and any(k.arg == "default" for k in d.keywords)
        ]
        is_default = not any(d == "False" for d in default_settings)

        # TODO: settings
        settings = []

        return Rule(name, description, patterns, is_default, settings)

    with open(file) as f:
        tree = ast.parse(f.read())

    # find the rules function using a matcher
    rules_def_func = None
    for node in ast.walk(tree):
        if isinstance(node, ast.FunctionDef) and node.name == "rules":
            rules_def_func = node
            break

    if rules_def_func is None:
        return []

    rules = [get_rule(r) for r in rules_def_func.body]
    return list([r for r in rules if r is not None])

def rst_rules(rules):
    return "\n".join([r.rst() for r in rules])

def output_rules(rules: typing.List[Rule], output_dir: str):
    # remove the existing output directory
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.makedirs(output_dir)

    # output the rules file
    with open(os.path.join(output_dir, "rules.rst"), "w") as f:
        f.write(rst_rules(rules))


def main():
    a = ap.ArgumentParser()
    a.add_argument("-r", "--rules", default=[], action='append', help="Rules to generate documentation for")
    a.add_argument("-o", "--output", default="chplcheck-rules-out", help="Directory where all the relevant docs files will be written")
    a.add_argument("--examples-directory", default=None, help="Directory where all the relevant examples are located")
    args = a.parse_args()

    rules: typing.List[Rule] = []

    # collect the rules
    for rule_file in args.rules:
        rules.extend(find_rules(rule_file))

    # collect the examples
    if args.examples_directory:
        for rule in rules:
            rule.add_example(args.examples_directory)

    # output the rules
    output_rules(rules, args.output)


if __name__ == "__main__":
    main()
