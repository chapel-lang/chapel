#!/usr/bin/env python3

'''
Report or reason about chpl test (compilation, execution, directory) times.
'''

import re
import argparse

def create_parser():
    ''' Create argument parser '''
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--timefile', help='file with test/dir timings', required=True)
    parser.add_argument('-n', type=int, default=10, help='number of tests/dirs to report')
    parser.add_argument('--sort-list', nargs='+', help='sort list of tests/dirs based on times from timefile')

    parser.set_defaults(mode='execution')
    parser.add_argument('--compilation', action='store_const', dest='mode', const='compilation')
    parser.add_argument('--execution',   action='store_const', dest='mode', const='execution')
    parser.add_argument('--directory',   action='store_const', dest='mode', const='directory')

    return parser


def get_pattern(mode):
    ''' Get regex pattern used to extract (test/dir, time) data '''
    if mode =='directory':
        pattern = re.compile(r'\[Finished subtest "(.*)" - (.*) seconds')
    else:
        pattern = re.compile(r'\[Elapsed {} time for "(.*)" - (.*) seconds'.format(mode))
    return pattern


def sort_chpl_tests(tests, timing_file, mode):
    '''
    Sort a lists of tests (comp, exec, or dir according to mode) using
    historical data in the timing file
    '''
    try:
        # Pull timings into dict, use to sort tests, put unknown tests early
        pattern = get_pattern(mode)
        with open(timing_file, 'r') as f:
            times = {m.group(1): float(m.group(2)) for m in pattern.finditer(f.read())}
        tests.sort(reverse=True, key=lambda test: times.get(test.lstrip('./'), 1000.0))
    except FileNotFoundError:
        pass

    return tests


def _main():
    args = create_parser().parse_args()

    if args.sort_list:
        print(' '.join(sort_chpl_tests(args.sort_list, args.timefile, args.mode)))
    else:
        with open(args.timefile, 'r') as f:
            pattern = get_pattern(args.mode)
            res = [(float(m.group(2)), m.group(1)) for m in pattern.finditer(f.read())]
            res.sort(reverse=True)

        print('Top {} {}:'.format(args.n, args.mode))
        for r in res[:args.n]:
            print(r)

if __name__ == '__main__':
    _main()



