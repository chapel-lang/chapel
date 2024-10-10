
import argparse as ap
import glob
import os

def get_futures(files):
    future_files = [f for f in files if f.endswith('.future')]
    return future_files

def find_all_futures(root):
    future_files = glob.glob(os.path.join(root, '**', '*.future'))
    return future_files

def check_future_file(file):
    """
    future files should have the following format:
    ```
    label: short description
    #<issue number>
    ```

    This returns (label, description, issue_number) if the file is valid, otherwise None
    """
    labels = ["bug", "error message", "feature request", "performance", "design", "portability", "unimplemented feature"]

    with open(file, 'r') as f:
        lines = f.readlines()
        if len(lines) < 3:
            print(f'{file} is missing a label, description, or issue number')
            return

        label = lines[0].split(':')[0].strip()
        if label not in labels:
            print(f'{file} has an invalid label: {label}')
            return

        description = lines[0].split(':')[1].strip()
        if len(description) == 0:
            print(f'{file} is missing a description')
            return

        issue_number = lines[1].strip()
        if not issue_number.startswith('#') or not issue_number[1:].isdigit():
            print(f'{file} is missing an issue number')
            return
        issue_number = int(issue_number[1:])

        return (label, description, issue_number)

def check_bad_files(root):
    """check that all bad files have an associated future"""

    bad_files = glob.glob(os.path.join(root, '**', '*.bad'))
    future_files = find_all_futures(root)
    bad_file_basename = [os.path.basename(bf).removesuffix('.bad') for bf in bad_files]
    future_file_basename = [os.path.basename(ff).removesuffix('.future') for ff in future_files]

    # check that all bad files have an associated future
    for bf in bad_file_basename:
        if bf not in future_file_basename:
            print(f'{bf}.bad has no associated future file')


def main():

    p = ap.ArgumentParser()
    p.add_argument('files', nargs='*')
    p.add_argument('--chpl-home', default=None)
    p.add_argument('--all', action='store_true', help='Check all files in the project, regardless of files specified')
    p.add_argument('--deleted-files', nargs='*', help='list of deleted files')
    args = p.parse_args()

    chpl_home = args.chpl_home
    if chpl_home is None:
        chpl_home = os.environ.get('CHPL_HOME', None)
    if chpl_home is None:
        raise ValueError('Must specify --chpl-home or set CHPL_HOME')

    test_dir = os.path.join(chpl_home, 'test')

    print(args.deleted_files)

    if args.all:
        future_files = find_all_futures(test_dir)
    else:
        future_files = get_futures(args.files)

    # build map of future files to their contents, also checks for format errors
    future_map = dict()
    for ff in future_files:
        future_map[ff] = check_future_file(ff)
    if any([v is None for v in future_map.values()]):
        exit(1)

    # make sure that all futures have an existing issue
    for ff, future in future_map.items():
        if future is None:
            continue
        label, description, issue_number = future
        # TODO: how to do this?

    # if any of the deleted files are futures, make sure their issue is closed
    # TODO: how do we do this? usually closing the PR closes the issue, but at
    # the time this check is run the issue is not yet closed


    if args.all:
        check_bad_files(test_dir)

if __name__ == '__main__':
    main()
