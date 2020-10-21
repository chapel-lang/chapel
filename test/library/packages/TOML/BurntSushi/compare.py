#!/usr/bin/env python3
# encoding: utf-8

from __future__ import print_function

"""Perform a diff of 2 JSON files. Exit 0 for success, 1 otherwise"""

import json
import os
import sys

"""Load 2 JSON files and diff them. Return 0 for success, 1 otherwise"""
def main():
    file1 = sys.argv[1]
    file2 = sys.argv[2]

    if not os.path.isfile(file1):
        print(file1)
        print('Unable to find good json file - this should not happen...')
        return 1

    if not os.path.isfile(file2):
        print(file2)
        print('Unable to find json file - test likely halted')
        return 1

    with open(file1, 'r') as f:
        try:
            json1 = json.load(f)
        except ValueError:
            print(file1)
            print('Unable to parse good json file - this should not happen...')
            return 1

    with open(file2, 'r') as f:
        try:
            json2 = json.load(f)
        except ValueError:
            print(file2)
            print('Unable to parse json file - test likely halted')
            return 1

    if json1 == json2:
        return 0

    print(file1)
    json_diff(json1, json2)
    return 1


"""Produce a diff of 2 JSON structures"""
def json_diff(json1, json2):
    set1 = set(json1)
    set2 = set(json2)
    intersect = set1.intersection(set2)

    diff1 = list(set1 - set2)
    diff2 = list(set2 - set1)
    valuediff = list(set(o for o in intersect if json2[o] != json1[o]))

    if diff1 or diff2:
        print('key diff:')
        for d in diff1:
            print('+', d)

        for d in diff2:
            print('-', d)

    if valuediff:
        print('key value:')
        for d in valuediff:
            print(' ', json1[d])
            print('-', json2[d])


if __name__ == '__main__':
    exit(main())
