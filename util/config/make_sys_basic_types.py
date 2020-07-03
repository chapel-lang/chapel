#!/usr/bin/env python

"""Build SysCTypes.chpl module.

Determines the size of various C types (e.g. long, size_t, etc) and creates a
Chapel module with 'extern types' declaration so Chapel code can refer to the
types.

Specify the output file as a positional argument, e.g.:

    %prog path/to/SysCTypes.chpl
"""

from __future__ import print_function

import contextlib
import logging
import optparse
import os
import os.path
import re
import subprocess
import sys


# List of C types. The items in each element are:
#
#  * C macro for maximum value for type
#  * Name of Chapel type
#  * Name of C type
_types = [
    ('INT_MAX', 'c_int', 'int'),
    ('UINT_MAX', 'c_uint', 'uint'),
    ('LONG_MAX', 'c_long', 'long'),
    ('ULONG_MAX', 'c_ulong', 'unsigned long'),
    ('LLONG_MAX', 'c_longlong', 'long long'),
    ('ULLONG_MAX', 'c_ulonglong', 'unsigned long long'),
    ('CHAR_MAX', 'c_char', 'char'),
    ('SCHAR_MAX', 'c_schar', 'signed char'),
    ('UCHAR_MAX', 'c_uchar', 'unsigned char'),
    ('SHRT_MAX', 'c_short', 'short'),
    ('USHRT_MAX', 'c_ushort', 'unsigned short'),
    ('INTPTR_MAX', 'c_intptr', 'intptr_t'),
    ('UINTPTR_MAX', 'c_uintptr', 'uintptr_t'),
    ('PTRDIFF_MAX', 'c_ptrdiff', 'ptrdiff_t'),
    ('SSIZE_MAX', 'ssize_t', 'ssize_t'),
    ('SIZE_MAX', 'size_t', 'size_t'),
]

# Map of max values to chapel types.
_max_value_to_chpl_type = {
    '127': 'int(8)',
    '255': 'uint(8)',
    '32767': 'int(16)',
    '65535': 'uint(16)',
    '2147483647': 'int(32)',
    '4294967295': 'uint(32)',
    '9223372036854775807': 'int(64)',
    '18446744073709551615': 'uint(64)',
}

_h_file_header = """
#include "sys_basic.h"
#include <limits.h>
#include <stdint.h>
#include <math.h>

FIND_INT_SIZES_START
"""


def main():
    """Parse command line arguments, create module, and print to stdout."""
    args = _parse_args()
    _setup_logging(args.verbose)

    # Get the module content first, since it can lead to errors/early exits if
    # something goes wrong.
    module_content = get_sys_c_types(args.doc)
    with open(args.output_file, 'w') as fp:
        fp.write(module_content)
        fp.write('\n')
    logging.debug('Wrote module to: {0}'.format(args.output_file))


def get_sys_c_types(docs=False):
    """Returns a string with the SysCTypes.chpl module content."""

    # Find the $CHPL_HOME/util/config/ dir.
    util_cfg_dir = os.path.abspath(os.path.dirname(__file__))
    logging.debug('Util config dir: {0}'.format(util_cfg_dir))

    # Get the C compile line.
    compileline_cmd = os.path.join(util_cfg_dir, 'compileline')

    compileline_env = os.environ.copy()
    # Make CHPL_COMM=none for the compile line command, we dont want any of the
    # stuff from gasnet (we dont want to require gasnet to be built)
    compileline_env['CHPL_COMM'] = 'none'
    # We need to clear CHPL_MAKE_CHPLENV_CACHE for our change to
    # CHPL_COMM to actually work
    compileline_env.pop('CHPL_MAKE_CHPLENV_CACHE', None)
    compileline_proc = subprocess.Popen([compileline_cmd, '--compile'],
        stdout=subprocess.PIPE, env=compileline_env)
    compileline = compileline_proc.communicate()[0].decode("utf-8").strip();
    logging.debug('Compile line: {0}'.format(compileline))

    # Create temp header file with *_MAX macros, then run it through the C
    # preprocessor using the C compile line found above.
    h_file = 'find_int_sizes_tmp.h'
    with _ensure_deleted(h_file):
        logging.debug('Creating temp header: {0}'.format(h_file))
        with open(h_file, 'w') as fp:
            fp.write(_h_file_header)

            for max_macro, _, _ in _types:
                fp.write('{0}\n'.format(max_macro))
            logging.debug('Wrote {0} max types to {1}'.format(len(_types), h_file))

        compile_cmd = '{compileline} -E {h_file}'.format(**locals())
        logging.debug('Preprocessor command: {0}'.format(compile_cmd))
        compile_result = os.popen(compile_cmd).read()
        logging.debug('Captured preprocessor output with {0} '
                      'characters.'.format(len(compile_result)))

    # Iterate through the preprocessor output, find the max value expressions,
    # and record them in a list.
    max_exprs = []
    keep = False
    for line in compile_result.splitlines():
        # Skip lines until the start macro is found.
        if not keep and 'FIND_INT_SIZES_START' not in line:
            continue

        # Found the start of the max macros. All subsequent lines will be
        # recorded.
        elif 'FIND_INT_SIZES_START' in line:
            keep = True

        # Ignore lines starting with # since they could be #line
        # type directives.
        elif line.strip().startswith("#"):
            continue

        # Ignore blank lines
        elif line.strip() == '':
            continue

        # The start of the max macros has already been found. Record every
        # line, stripping it of whitespace.
        else:
            max_exprs.append(line.strip())
    logging.debug('Found {0} lines of max type '
                  'values.'.format(len(max_exprs)))

    if len(max_exprs) != len(_types):
        logging.error('Found {0} max values, but {1} types were '
                      'expected.'.format(len(max_exprs), len(_types)))
        sys.exit(1)

    # Iterate through the max value expressions, evaluate each one, and store
    # it in a list. Python deals with arbitrarily large integers, so there is
    # no fear of overflow.
    replace_pattern = re.compile(r'[UL]', re.IGNORECASE)
    max_values = []
    for expr in max_exprs:
        ex = re.sub(replace_pattern, '', expr)
        value = eval(ex)
        logging.debug('{0} -> {1}'.format(expr, value))
        max_values.append(value)
    logging.debug('Evaluated all {0} expressions from '
                  'preprocessor.'.format(len(max_values)))

    # Iterate through the chapel types/max values and print out the SysCTypes
    # Chapel module code. Each line takes the form "extern type <chpl_type>=
    # <chpl_value>;" where <chpl_value> is found by looking up the max value
    # (from evaluated expression above) in the _max_value_to_chpl_type map.
    sys_c_types = []
    handled_c_ptr = False
    for i, max_value in enumerate(max_values):
        max_macro, chpl_type, c_type = _types[i]
        chpl_value = _max_value_to_chpl_type.get(str(max_value))
        if chpl_value is None:
            logging.error('Unknown numeric limit {0} in '
                          '_max_value_to_chpl_type dict.'.format(max_value))
            sys.exit(1)

        sys_c_types.append('/* The type corresponding to the C {c_type} type'
                           ' */'.format(**locals()))
        stmt = 'extern type {chpl_type}= '.format(**locals())
        if docs:
            stmt += 'integral'
        else:
            stmt += chpl_value
        stmt += ';'
        sys_c_types.append(stmt)

        if chpl_type == 'c_ptr':
            handled_c_ptr = True

    # never emit c_void_ptr since it is a built-in type now
    #if not handled_c_ptr:
    #    sys_c_types.append('extern type c_void_ptr; '
    #                       '// opaque; no ptr arithmetic in Chapel code!')

    # Finally, print out set of asserts for module. They assert that the
    # sizeof(<extern chpl type>) matches the sizeof(<chpl type>). E.g.
    #
    #   assert(sizeof(c_int) == sizeof(int(32)));
    #
    sys_c_types.append("""
{
  extern proc sizeof(type t): size_t;
""")
    for i, max_value in enumerate(max_values):
        _, chpl_type, _ = _types[i]
        chpl_value = _max_value_to_chpl_type.get(str(max_value))
        sys_c_types.append('  assert(sizeof({chpl_type}) == sizeof({chpl_value}))'
              ';'.format(**locals()))
    sys_c_types.append('}')

    return '\n'.join(sys_c_types)


@contextlib.contextmanager
def _ensure_deleted(filename):
    """Ensure file is deleted after context manager."""
    try:
        yield
    finally:
        if os.path.exists(filename):
            logging.debug('Deleting: {0}'.format(filename))
            os.unlink(filename)


def _parse_args():
    """Parse and return command line args."""
    class NoWrapHelpFormatter(optparse.IndentedHelpFormatter):
        """Help formatter that does not wrap the description text."""

        def _format_text(self, text):
            return text

    parser = optparse.OptionParser(
        usage='usage: %prog [--doc] [--verbose] <SysCTypes_filename>',
        description=__doc__,
        formatter=NoWrapHelpFormatter()
    )

    parser.add_option(
        '-v', '--verbose', action='store_true',
        help='Enable verbose output.'
    )
    parser.add_option(
        '--doc', action='store_true',
        help='Build SysCTypes module for chpldoc.'
    )

    opts, args = parser.parse_args()

    if len(args) != 1:
        parser.print_help()
        sys.exit(1)
    else:
        opts.output_file = args[0]

    return opts


def _setup_logging(verbose=False):
    """Initialize logging and set level based on verbose.

    :type verbose: bool
    :arg verbose: When True, set log level to DEBUG.
    """
    log_level = logging.DEBUG if verbose else logging.WARNING
    logging.basicConfig(format='%(levelname)s: %(message)s',
                        level=log_level)
    logging.debug('Verbose output enabled.')


if __name__ == '__main__':
    main()
