#!/usr/bin/env python3

"""
Replace substitutable strings starting with // in templates.
"""

import argparse, sys
from string import Template

def create_parser():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--template', type=argparse.FileType('r'),
                        help='The input template',
                        default=sys.stdin)
    parser.add_argument('--output', type=argparse.FileType('w'),
                        help='The output file with substitutions made',
                        default=sys.stdout)
    return parser

def cli_to_dict(keys):
    substitutions = {}
    for arg in keys:
        words = arg.split('=', 1)
        substitutions[words[0]] = words[1]
    return substitutions

# Use // as the delimiter
class SlashTemplate(Template):
    delimiter = '@@'

def _main():
    args, keys = create_parser().parse_known_args()

    substitutions = cli_to_dict(keys)
    src = SlashTemplate(args.template.read())
    result = src.substitute(substitutions)
    args.output.write(result)

if __name__ == '__main__':
    _main()
