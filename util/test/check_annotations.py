#!/usr/bin/env python

import annotate

import os
import re
import subprocess
import sys
import time
import traceback

#
# This is a kinda-ugly script that does basic validation of ANNOTATIONS.yaml
# since we always manage to screw it up and cause nightly testing noise.
#
# It does the following:
#  - "parses" all annotations by calling annotate.get() for each .graph listed
#    in $CHPL_HOME/test/*GRAPHFILES for all date ranges.
#  - checks that all the "group" (graph) names listed in the annotations file
#    have corresponding .graph files
#  - checks that all the configs used in the annotations file are "known"
#  - checks that annotation dates are not the same as the merge date
#    (annotation dates are supposed to be for the first "affected" date
#
# TODOs:
#  - make a non-stop mode (currently it exits on the first error)
#    - and have better error/warning reporting
#  - add some form of testing (maybe like what we do for updateDatFiles.py with
#    performance/compiler/elliot/updateDatFiles)
#  - run this as part of the smoke test
#

def main():
    chpl_home = os.environ.get('CHPL_HOME')
    test_dir = os.path.join(chpl_home, 'test')
    ann_path=os.path.join(test_dir, 'ANNOTATIONS.yaml')

    ann_data = annotate.load(ann_path)
    graph_list = get_graph_names(test_dir)

    check_graph_names(ann_data, graph_list)
    try_parsing_annotations(ann_data, graph_list)
    check_configs(ann_data)
    check_pr_number_dates(ann_data)

    print("\nNo fatal annotation errors detected")

# Parse $CHPL_HOME/test/*GRAPHFILES and return the basenames of all .graph
# files listed
def get_graph_names(test_dir):
    graph_list = []
    GRAPHFILES_files = [f for f in os.listdir(test_dir) if f.endswith("GRAPHFILES")]
    for GRAPHFILE in GRAPHFILES_files:
        with open (os.path.join(test_dir, GRAPHFILE), 'r') as f:
            for l in f.readlines():
		l = l.strip()
		if l.startswith('#') or len(l) == 0:
		    continue
		graph_list.append(os.path.basename(l).replace('.graph', ''))
    return graph_list

# Parse a string into a date
def parse_date(value, dateformat='%Y-%m-%d'):
    return time.strptime(value.strip(), dateformat)

# Parse annotations for every possible graph for all valid date ranges
def try_parsing_annotations(ann_data, graph_list):
    for graph in graph_list:
        try:
            annotate.get(ann_data, graph, '', parse_date('2001-01-01'), parse_date('2100-12-31'), '')
        except:
            print('Error parsing annotations for "{0}"\n'.format(graph))
            traceback.print_exc()
            exit(-1)

# Check that all graph names listed in the annotation file have an associated
# .graph in $CHPL_HOME/test/*GRAPHFILES
def check_graph_names(ann_data, graph_list):
    for graph in ann_data:
        if graph != 'all' and graph not in (graph_list):
            print('Warning: no .graph file found for "{0}"'.format(graph))

# Check that all the configs used in the annotation file are "known"
def check_configs(ann_data):
    known_configs = {'shootout', 'chap03', 'chap04', 'bradc-lnx', 'chapcs',
                     '16 node XC', 'Single node XC'}
    for graph in ann_data:
        for date, annotations in ann_data[graph].iteritems():
            for ann in annotations:
                if isinstance(ann, dict) and 'config' in ann:
                    configs = ann['config'].split(',')
                    for config in configs:
                        config = config.strip()
                        if config not in known_configs:
                            print('Warning: config "{0}" for graph "{1}" not recognized'.format(config, graph))

# Helper function to compute a map of PR numbers to commit dates
def compute_pr_to_dates():
    pr_to_date_dict = {}
    git_cmd = 'git log --grep "^Merge pull request #" --date=short --pretty=format:"%ad ::: %s"'
    p = subprocess.Popen(git_cmd, stdout=subprocess.PIPE, shell=True)
    git_log = p.communicate()[0]
    for line in git_log.splitlines():
        split_line = line.split(' ::: ')
        date = split_line[0]
        pr_num = re.match(r'.*#(\d+)', split_line[1]).group(1)
        pr_to_date_dict[pr_num] = parse_date(date)

    return pr_to_date_dict

# Check annotation dates aren't the same as the date the PR was committed. This
# check could be beefed up to check that dates are within a week or so of the
# commit date
def check_pr_number_dates(ann_data):
    pr_to_date_dict = compute_pr_to_dates()
    for graph in ann_data:
        for date, annotations in ann_data[graph].iteritems():
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
                            print('Warning: annotation date for "{0}: {1}" appears to be the same as the commit date'.format(graph, text))


if __name__ == '__main__':
    sys.exit(main())
