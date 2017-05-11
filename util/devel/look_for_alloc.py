#!/usr/bin/env python

"""Simple script that effectively "greps" for calls to the system allocator.
For projects that provide their own allocation interface, this helps ensure
that direct calls to the system allocator aren't accidentally introduced.

Finds all C/C++ files in the search directory, and uses cscope to check for and
report calls to system allocation/deallocation routines.

Prints out the location of any calls to the system allocator. Exit status is 0
on success, 1 if calls to the allocator were found, and 2 if there were fatal
errors trying to run this script.
"""

import fnmatch
import optparse
import os
import subprocess
import sys

def log_error(msg, fatal=False):
    """Log an error, exiting if fatal"""
    sys.stdout.write('Error: {0}\n'.format(msg))
    if fatal:
        sys.exit(2)


def run_command(cmd):
    """Simple subprocess wrapper. Similar to subprocess.check_output, but that
       is only available after Python 2.7, and we still support 2.6 :("""
    try:
        process = subprocess.Popen(cmd,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE)
    except OSError:
        log_error('command not found: "{0}"'.format(cmd[0]), fatal=True)

    output = process.communicate()
    output = (output[0].decode(), output[1].decode())
    if process.returncode != 0:
        msg = 'command failed: "{0}"\nstderr was:\n{1}'.format(cmd, output[1])
        log_error(msg, fatal=True)
    else:
        return output[0]


def get_alloc_funcs():
    """Return a list of the possible C alloc/dealloc routines"""
    std = ['malloc', 'calloc', 'realloc', 'free']
    align = ['aligned_alloc', 'posix_memalign', 'memalign']
    page_align = ['valloc', 'pvalloc']
    string = ['strdup', 'strndup', 'asprintf', 'vasprintf']
    obscure = ['getline', 'getdelim']

    return std + align + page_align + string + obscure


def find_files(search_dir, extensions):
    """Return a list of absolute paths to files with any of the provided
       extensions in the search_dir."""
    source_files = []
    for root, _, filenames in os.walk(search_dir):
        for ext in extensions:
            for filename in fnmatch.filter(filenames, '*.{0}'.format(ext)):
                source_files.append(os.path.join(root, filename))
    return source_files


def find_source_files(search_dir):
    """Return a list of absolute paths to any C/C++ source files in
       the search_dir"""
    cpp_sources = ['cc', 'cp', 'cxx', 'cpp', 'CPP', 'c++', 'C']
    cpp_headers = ['hh', 'H', 'hp', 'hxx', 'hpp', 'HPP', 'h++', 'tcc']
    c_sources = ['c']
    c_headers = ['h']
    code_file_exts = set(cpp_sources + cpp_headers + c_sources + c_headers)
    return find_files(search_dir, code_file_exts)


def build_cscope_ref(src_files):
    """Build a cscope cross-reference for crazy fast searching of src_files"""
    # -bu -- build cross ref only, wipe any old cross refs
    # -q  -- enable faster symbol lookup
    # -k  -- turns off default include dir (don't include/parse system files
    #        since we only care if our files call the system allocator)
    cscope_cmd = ['cscope', '-bu', '-q', '-k'] + src_files
    run_command(cscope_cmd)


def cscope_find_calls(func_call):
    """Run cscope searching for calls to func_call"""
    # -d  -- don't rebuild the cross-reference, use the one we already built
    # -L3 -- line based "Find functions calling this function"
    cscope_cmd = ['cscope', '-d', '-L3{0}'.format(func_call)]
    return run_command(cscope_cmd)


def check_for_alloc_calls(search_dir, exclude_paths=None, rel_paths=True):
    """Check source files in search_dir for calls to the system allocator.
       Don't check files that contain any of the exclude_paths.  Report files
       relative to search_dir/../ if rel_paths is True, otherwise use absolute
       paths. Returns True if alloc calls were found, False otherwise"""
    rel_dir = ''
    if rel_paths:
        rel_dir = os.path.abspath(os.path.join(search_dir, '..')) + os.path.sep

    src_files = find_source_files(search_dir)
    if exclude_paths:
        for exclude_path in exclude_paths:
            src_files = [s for s in src_files if exclude_path not in s]

    build_cscope_ref(src_files)

    found_alloc_calls = False
    for alloc_func in get_alloc_funcs():
        out = cscope_find_calls(alloc_func)
        if out:
            found_alloc_calls = True
            log_error('found call to "{0}"'.format(alloc_func))
            sys.stdout.write(out.replace(rel_dir, '') + '\n')
    return found_alloc_calls


def main():
    """Parse options and check for alloc calls"""

    class MyParser(optparse.OptionParser):
        """Optparse wrapper that doesn't strip newlines from the epilog"""
        def format_epilog(self, formatter):
            return self.epilog

    parser = MyParser(epilog='\n{0}'.format(__doc__))
    parser.add_option('--search-dir', dest='search_dir', default=os.getcwd(),
                      help='directory to check for alloc calls [default: CWD]')
    parser.add_option('--exclude-paths', dest='exclude_paths', default='',
                      help='comma separated list of (sub)paths/files to skip')
    parser.add_option('--abs-paths', dest='abs_paths', action="store_true",
                      help='report abs paths vs. rel to --search-dir/../')
    options = parser.parse_args()[0]

    search_dir = os.path.abspath(options.search_dir)
    exclude_paths = [x.strip() for x in options.exclude_paths.split(',') if x]
    rel_paths = not options.abs_paths

    return check_for_alloc_calls(search_dir, exclude_paths, rel_paths)


if __name__ == "__main__":
    sys.exit(main())
