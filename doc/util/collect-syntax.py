#!/usr/bin/env python3
# encoding: utf-8


"""
collect syntax
"""


from __future__ import print_function
import argparse
import os
import re
import sys
import stat


# Regular expressions to identify productions within RST files.
_capture_space = re.compile(r"(\s*)\S.*")
_capture_chapter = re.compile(r"\s*.. _Chapter-([a-zA-Z0-9._-]+):\s*")
_capture_testname = re.compile(r"\s*[*]?Example\s+\(([a-zA-Z0-9._-]+)\).*")
_match_code = re.compile(r"\s*.. code-block:: chapel\s*|" +
                        r"\s*.. BLOCK-test-chapelnoprint\s*|" +
                        r"\s*.. BLOCK-test-chapelpost\s*|" +
                        r"\s*.. BLOCK-test-chapelpre\s*")
_match_outputname = re.compile(r"\s*.. BLOCK-test-chapeloutputname\s*")
_match_output = re.compile(r"\s*.. BLOCK-test-chapeloutput\s*|" +
                          r"\s*.. .. code-block:: printoutput")
_match_compopts = re.compile(r"\s*.. BLOCK-test-chapelcompopts\s*")
_match_execopts = re.compile(r"\s*.. BLOCK-test-chapelexecopts\s*")
_match_prediff = re.compile(r"\s*.. BLOCK-test-chapelprediff\s*")
_match_function = re.compile(r"\s*.. function::\s*")


# Global variables...
_productions = {}
_rstfiles = None
_outdir = None
_outfile = None
_verbose = False


def get_script_args():
  parser = argparse.ArgumentParser(
      prog='collect-syntax',
      usage='%(prog)s file.rst [options]',
      description=__doc__,
      formatter_class=argparse.RawDescriptionHelpFormatter)
  parser.add_argument(
      'rstfiles',
      nargs='+',
      help='gather syntax rules from these RST files')
  parser.add_argument(
      '--output',
      default='doc/rst/language/spec/',
      help='destination directory of output')
  parser.add_argument(
      '--verbose',
      '-v',
      action='store_true',
      default=False,
      help='verbosity')
  return parser.parse_args()


def print_verbose(*args):
  if verbose:
    print(*args)
  return


def collect_productions(rstfile):
  print_verbose('Processing .rst file: ', rstfile)
  lines = None
  with open(rstfile, 'r', encoding='utf-8') as fp:
    lines = fp.readlines()


def collect_productions_from_files():
  for rstfile in _rstfiles:
    if os.path.isfile(rstfile):
      collect_productions(rstfile)
      continue
    if os.path.isdir(rstfile):
      for dirpath, dirnames, filenames, in os.walk(rstfile):
        for f in filenames:
          if f.endswith('.rst'):
            path = os.path.join(dirpath, f)
            collect_productions(path)
      continue
    msg = 'File not found: ' + str(rstfile)
    print_verbose(msg)
    error = Exception(msg)
    raise error


# Use a triple escaped string to neatly capture prelude text blob.
_prelude_text = """.. default-domain:: chpl

.. _Chapter-Syntax:

Collected Syntax Productions
============================

This appendix collects the syntax productions listed throughout the
language specification. There are no new syntax productions listed
in this appendix. The productions are listed both alphabetically and
in application order for convenience.

"""

def output_prelude(out):
  out.write(_prelude_text)
  return

def output_productions_alphabetical(out):
  out.write('.. _Productions_Listed_Alphabetically\n')
  out.write('\n')
  out.write('Productions Listed Alphabetically\n')
  out.write('---------------------------------\n')
  out.write('\n')
  out.write('\n')
  return


def output_productions_application_order(out):
  out.write('.. _Productions_Listed_Application_Order\n')
  out.write('\n')
  out.write('Productions Listed in Application Order\n')
  out.write('---------------------------------------\n')
  out.write('\n')
  out.write('\n')
  return


def main(**kwargs):
  global _rstfiles, _output, _verbose
  _rstfiles = kwargs['rstfiles']
  _outdir = kwargs['outdir']
  _rstfiles = kwargs['rstfiles']
  collect_productions_from_files()
  with open(os.path.join(_outdir, 'syntax.rst'), 'w') as outfile:
    output_prelude(outfile)
    output_productions_alphabetical(outfile)
    output_productions_application_first(outfile)
  return


if __name__ == '__main__':
  err = 0
  try:
    # Parse arguments and cast them into a dictionary.
    arguments = vars(get_arguments())
    main(**arguments)
  except Exception as e:
    print(e)
    err = 1
  sys.exit(err)
  pass

