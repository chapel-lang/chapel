#!/usr/bin/env python
""" Do basic validation and checks on the ANNOTATIONS.yaml file

This is a kinda-ugly script that does basic validation of ANNOTATIONS.yaml
since we always manage to screw it up and cause nightly testing noise.

It does the following:
 - "parses" all annotations by calling annotate.get() for each .graph listed
   in $CHPL_HOME/test/*GRAPHFILES for all date ranges.
 - checks that all the "group" (graph) names listed in the annotations file
   have corresponding .graph files
 - checks that all the configs used in the annotations file are "known"
 - checks that annotation dates are not the same as the merge date
   (annotation dates are supposed to be for the first "affected" date
 """

from __future__ import print_function

import os
import re
import subprocess
import sys
import time
import warnings

import annotate

chplenv_dir = os.path.join(os.path.dirname(__file__), '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

from chpl_home_utils import get_chpl_home

def main():
    """
    Parse and do some basic validation of the ANNOTATIONS.yaml file
    """
    os.environ["TZ"] = "America/Los_Angeles"
    chpl_home = get_chpl_home()
    test_dir = os.path.join(chpl_home, 'test')
    ann_path = os.path.join(test_dir, 'ANNOTATIONS.yaml')

    with warnings.catch_warnings(record=True) as warnings_list:
        ann_data = annotate.load(ann_path)
        graph_list = get_graph_names(test_dir)

        check_graph_names(ann_data, graph_list)
        try_parsing_annotations(ann_data, graph_list)
        check_configs(ann_data)
        check_pr_number_dates(ann_data)

        for warning in warnings_list:
            print(warning.message)
        print('No fatal annotation errors detected')
        return len(warnings_list) != 0


def get_graph_names(test_dir):
    """Parse test_dir/*GRAPHFILES and return basenames for all .graph files"""
    graph_list = []
    GRAPHFILES_files = [f for f in os.listdir(test_dir) if f.endswith("GRAPHFILES")]
    for GRAPHFILE in GRAPHFILES_files:
        with open(os.path.join(test_dir, GRAPHFILE), 'r') as f:
            for l in f.readlines():
                l = l.strip()
                if not l or l.startswith('#'):
                    continue
                graph_list.append(os.path.basename(l).replace('.graph', ''))
    return graph_list

def parse_date(value, dateformat='%Y-%m-%d'):
    """Parse a string into a date"""
    return time.strptime(value.strip(), dateformat)

def try_parsing_annotations(ann_data, graph_list):
    """Parse annotations for every possible graph for all valid date ranges"""
    for graph in graph_list:
        try:
            start_date = parse_date('2001-01-01')
            end_date = parse_date('2100-12-31')
            annotate.get(ann_data, graph, '', start_date, end_date, '')
        except Exception as e:
            print('Error parsing annotations for "{0}"\n'.format(graph))
            raise e

def check_graph_names(ann_data, graph_list):
    """Check that graph names in the annotation file are listed in GRAPHFILES"""
    for graph in ann_data:
        if graph != 'all' and graph not in graph_list:
            warnings.warn('Warning: no .graph file found for "{0}"'.format(graph))

def check_configs(ann_data):
    """Check that all the configs used in the annotation file are 'known'"""
    known_configs = {'shootout', 'chap03', 'chap04', 'bradc-lnx', 'chapcs',
                     '16-node-xc', '1-node-xc', '16-node-cs',
                     'chapcs.comm-counts'}
    for graph in ann_data:
        for _, annotations in ann_data[graph].items():
            for ann in annotations:
                if isinstance(ann, dict) and 'config' in ann:
                    configs = ann['config'].split(',')
                    for config in configs:
                        config = config.strip()
                        if config not in known_configs:
                            warnings.warn('Warning: unrecognized config "{0}" '
                                          'for graph "{1}"'.format(config, graph))

def compute_pr_to_dates():
    """Helper function to compute a map of PR numbers to commit dates"""
    pr_to_date_dict = {}
    git_cmd = 'git log --grep "^Merge pull request #" --date=short-local --pretty=format:"%ad ::: %s"'
    p = subprocess.Popen(git_cmd, stdout=subprocess.PIPE, shell=True)
    git_log = p.communicate()[0]
    if sys.version_info[0] >= 3 and not isinstance(git_log, str):
        git_log = str(git_log, 'utf-8')
    for line in git_log.splitlines():
        split_line = line.split(' ::: ')
        date = split_line[0]
        pr_num = re.match(r'Merge pull request #(\d+)', split_line[1]).group(1)
        pr_to_date_dict[pr_num] = parse_date(date)

    return pr_to_date_dict

def check_pr_number_dates(ann_data):
    """
    Check that annotation dates aren't the same as commit date. This could be
    beefed up to check that dates are within a week or so of the commit date
    """
    pr_to_date_dict = compute_pr_to_dates()
    for graph in ann_data:
        for date, annotations in ann_data[graph].items():
            for ann in annotations:
                if isinstance(ann, dict):
                    text = ann.get('text')
                else:
                    text = ann
                if '#' in text:
                    pr_num = re.match(r'.*#(\d+)', text).group(1)
                    if pr_num in pr_to_date_dict:
                        pr_date = pr_to_date_dict[pr_num]
                        if pr_date == date:
                            warnings.warn('Warning: annotation date for "{0}: '
                                          '{1}" appears to be the same as the '
                                          'commit date'.format(graph, text))


if __name__ == '__main__':
    sys.exit(main())
