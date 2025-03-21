#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Combine reports A.xml, B.xml, C.xml into a combined A.xml"""

import argparse
import os
import sys

from junitparser import JUnitXml, TestCase, TestSuite

def merge_junit_xmls(xml_files):
    if not xml_files:
        return

    merged = sum((JUnitXml.fromfile(xml) for xml in xml_files), JUnitXml())
    merged.write(xml_files[0])

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('xml_files', nargs='+', help='JUnit XML files to merge')
    args = parser.parse_args()
    merge_junit_xmls(args.xml_files)

if __name__ == '__main__':
    main()
