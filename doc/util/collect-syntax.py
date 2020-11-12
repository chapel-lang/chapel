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
import traceback


# Regular expressions to identify productions within RST files.
_match_syntax_begin = re.compile(r"\s*.. code-block:: syntax\s*\n")
_match_syntax_end = re.compile(r"\S+")
_match_production = re.compile(r" {3}(?P<ident>([a-zA-Z0-9._-]+)):.*")
_match_expansion = re.compile(r" {3} +")
_match_symbol = re.compile(r"\s*('[a-zA-Z0-0._-]+')|([a-zA-Z0-0._-]+)")

# Global variables...
_productions = {}
_do_list_productions = False
_rstfiles = None
_outdir = None
_outfile = None
_verbose = False


def get_script_arguments():
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
      '--outdir',
      default='doc/rst/language/spec/',
      help='destination directory of output')
  parser.add_argument(
      '--verbose',
      '-v',
      action='store_true',
      default=False,
      help='enable verbose output')
  parser.add_argument(
      '--list-productions',
      action='store_true',
      default=False,
      help='print all collected productions')
  return parser.parse_args()


def vprint(*args):
  global _verbose
  if _verbose:
    print(*args)
  return


def collect_productions(rstfile):
  lines = None
  with open(rstfile, 'r', encoding='utf-8') as fp:
    lines = fp.readlines()
  block_start_line = None
  result = []
  i = 0
  while i < len(lines):
    line = lines[i]
    if not block_start_line:
      if re.match(_match_syntax_begin, line):
        vprint('Syntax block begin on line:', (i + 1))
        block_start_line = i
      i += 1
      continue
    assert(block_start_line)
    match_prod = re.search(_match_production, line)
    while match_prod:
      ident = match_prod.group('ident')
      lineno = i
      vprint('Production', ident, 'on line', lineno)
      blob = line
      expansions = 0
      i += 1
      while i < len(lines):
        line = lines[i]
        if not re.match(_match_expansion, line):
          break
        expansions += 1
        blob += line
        i += 1
      if expansions == 0:
        msg = 'No RHS for \'' + str(ident) + '\' on line: ' + str(lineno)
        vprint(msg)
        raise Exception(msg)
      result.append((ident, lineno, blob))
      match_prod = re.search(_match_production, line)
    if re.match(_match_syntax_end, line) or (i + 1) >= len(lines):
      vprint('Syntax block end on line:', (i + 1))
      block_start_line = None
    i += 1
    pass
  if block_start_line:
    msg = 'Unterminated syntax block, file ' + rstfile
    msg += ' start line ' + str(block_start_line + 1)
    vprint(msg)
    raise Exception(msg)
  return result 


def get_all_files_to_process():
  result = []
  for rstfile in _rstfiles:
    if os.path.isfile(rstfile):
      if not os.path.basename(rstfile) == 'syntax.rst':
        result.append(rstfile)
      continue
    if os.path.isdir(rstfile):
      for dirpath, dirnames, filenames, in os.walk(rstfile):
        for f in filenames:
          if f.endswith('.rst'):
            path = os.path.join(dirpath, f)
            if not os.path.basename(path) == 'syntax.rst':
              result.append(path)
      continue
    msg = 'File or directory not found: ' + str(rstfile)
    vprint(msg)
    raise Exception(msg)
  return result 


def error_if_duplicate_rule(rstfile, ident, lineno):
  if ident in _productions:
    fname, lineno, _ = _productions[ident]
    msg = 'In ' + str(rstfile) + ', line ' + str(lineno)
    msg += 'Duplicate identifier for rule: ' + str(ident) + '\n'
    msg += 'First defined in ' + str(fname) + ' on line ' + str(lineno)
    vprint(msg)
    raise Exception(msg)
  return


def collect_productions_from_files():
  fnames = get_all_files_to_process()
  for fname in fnames:
    vprint('Processing file:', fname)
    collected = collect_productions(fname)
    vprint('Captured', len(collected), 'productions in', fname)
    for ident, lineno, blob in collected:
      global _productions
      error_if_duplicate_rule(fname, ident, lineno)
      _productions[ident] = (fname, lineno, blob)
  return


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
  out.write('.. _Productions_Listed_Alphabetical_Order:\n')
  out.write('\n')
  title = 'Productions Listed in Alphabetical Order'
  out.write(title + '\n')
  out.write((len(title) * '-') + '\n')
  out.write('\n')
  out.write('.. code-block:: syntax\n')
  out.write('\n')
  global _productions
  for ident in sorted(_productions.keys()):
    _, _, blob = _productions[ident]
    out.write(blob)
    out.write('\n')
    pass
  return


def is_grammar_keyword(ident):
  return ident == 'OPT'


def get_nonterminals_in_expansion(ident, stack):
  global _productions
  fname, lineno, blob = _productions[ident]
  vprint('Inspecting RHS of:', ident)
  nonterminals = []
  for line in blob.split('\n'):
    if re.match(_match_production, line):
      continue
    symbols = re.findall(_match_symbol, line)
    # Regex trick, second capture group contains non-quoted symbols.
    nonterminals += [group[1] for group in symbols if group[1]]
  vprint('Found', len(nonterminals), 'nonterminals')
  vprint(nonterminals)
  for ident in nonterminals:
    assert(ident)
    if not is_grammar_keyword(ident) and not ident in stack:
      stack.append(ident)
  return


def output_productions_application_order(out):
  out.write('.. _Productions_Listed_Application_Order:\n')
  out.write('\n')
  title = 'Productions Listed in Application Order'
  out.write(title + '\n')
  out.write((len(title) * '-') + '\n')
  out.write('\n')
  out.write('.. code-block:: syntax\n')
  out.write('\n')
  global _productions
  start = 'module-declaration-statement'
  vprint('Computing application-first ordering starting with:', start)
  stack = [start]
  applied = []
  count = 0
  while stack:
    ident = stack[0]
    if not ident in _productions:
      msg = 'Nonterminal not in production list: ' + str(ident)
      vprint(msg)
      raise Exception(msg)
    get_nonterminals_in_expansion(ident, stack)
    _, _, blob = _productions[ident]
    out.write(blob)
    out.write('\n')
    applied.append(stack.pop(0))
    count += 1
  # Consider unapplied nonterminals a fatal error...
  if count != len(_productions):
    unapplied_count = len(_productions) - count
    msg = str(unapplied_count) + ' nonterminals are never applied'
    vprint(msg)
    for ident in _productions.keys():
      if not ident in applied:
        vprint(ident)
    raise Exception(msg)
  return


def main(args):
  global _do_list_productions, _rstfiles, _outdir, _verbose
  _do_list_productions = args['list_productions']
  _rstfiles = args['rstfiles']
  _outdir = args['outdir']
  _verbose = args['verbose']
  collect_productions_from_files()
  if _do_list_productions:
    global _productions
    print('Collected', len(_productions))
    for ident in sorted(_productions.keys()):
      fname, lineno, blob = _productions[ident]
      print(ident, 'in', fname, 'on line', lineno)
      print(blob)
  outname = os.path.join(_outdir, 'syntax.rst')
  vprint('Writing output to:', outname)  
  with open(outname, 'w') as outfile:
    output_prelude(outfile)
    output_productions_alphabetical(outfile)
    output_productions_application_order(outfile)
  return


if __name__ == '__main__':
  err = 0
  try:
    # Parse arguments and cast them into a dictionary.
    arguments = vars(get_script_arguments())
    main(arguments)
  except Exception as e:
    traceback.print_exc()
    err = 1
  sys.exit(err)

