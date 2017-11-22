#!/usr/bin/env python3
# encoding: utf-8

"""
Sanity check script to confirm that python's json module can successfully
parse and compare json files in this test suite.

Developer script (python3 only)
"""

import compare

import json
import os


def main():
    json_files = [j for j in os.listdir('.') if j.endswith('.json')]

    for json_file in json_files:
        print('testing ', json_file)
        with open(json_file, 'r') as handle:
            json_data = json.load(handle)
        json_string = json.dumps(json_data)
        json_recycled = json.loads(json_string)

        if compare.json_diff(json_data, json_recycled):
            print('failed to parse json: ', json_file)

if __name__ == '__main__':
    main()
