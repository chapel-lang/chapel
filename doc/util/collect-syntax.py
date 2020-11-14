#!/usr/bin/env python3
# encoding: utf-8


"""

Read the RST files in '$CHPL_HOME/doc/rst/language/spec' and look for
any blocks of the form:

```
Normal text here...

.. code-block:: syntax
   production-1:
     production-2 production-1

   production-2:
     'foo'

Normal text here...
```

These are syntax blocks which describe the grammar of Chapel. The
script parses each found block and keys all productions by name.

The script outputs all productions in alphabetical order followed by
application order. Application order is a breadth-first ordering
rooted on the start symbol 'module-declaration-statement'.

The script outputs all productions into the file:

  '$CHPL_HOME/doc/rst/language/spec/syntax.rst'

This script expects the following formatting from syntax blocks:

  - Code in a syntax block must be indented at least one space
  - A syntax block ends upon hitting EOF or the first line to begin
    with a non-whitespace character
  - Within a block, lines containing only whitespace are ignored
  - Expansions on the RHS of a production must be indented more than
    the production's declaration

This script will complain if:

  - There is more than one production with the same name
  - A nonterminal has no corresponding production
  - A production is defined but never applied
  - A production has zero expansions (empty RHS)

"""


import argparse
import os
import re
import sys


# Group 1 = any amount of whitespace (all treated as spaces)
_capture_leading_spaces = re.compile(r"(\s*)")

# Group 1 = nonzero indentation | Group 2 = production name
_capture_production = re.compile(r"(?P<indent>\s+)" +
                                 r"(?P<symbol>([a-zA-Z0-9._-]+)):.*")

# Group 1 = 'quoted' | Group 2 = \escaped | Group 3 = nonterminal
_capture_nonterminal = re.compile(r"\s+('[ a-zA-Z0-9-()]+')|" +
                                  r"(\\[a-zA-Z]+)|" +
                                  r"([a-zA-Z0-9-]+)")

_match_syntax_begin = re.compile(r"(\s*).. code-block:: syntax\s*\n")
_match_syntax_end = re.compile(r"\S+")
_match_all_whitespace= re.compile(r"\s*\Z")

# Global variables...
_productions = {}
_rstfiles = None
_outdir = None
_outfile = None
_verbose = False
_start = 'module-declaration-statement'


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
  return parser.parse_args()


def vprint(*args):
  global _verbose
  if _verbose:
    print(*args)


class SyntaxParser(object):
  def __init__(self, fname):
    self._productions = []
    self._fname = fname
    self._lines = None
    with open(fname, 'r', encoding='utf-8') as fp:
      self._lines = fp.readlines()
    assert(self._lines)
    self._collect_productions()

  def _parse_production_blob(self, start):
    production = re.match(_capture_production, self._lines[start])
    assert(production)
    base_indent = len(production.group('indent'))
    symbol = production.group('symbol')
    fname = self._fname
    vprint('Production', symbol, 'at', fname, ':', (start + 1))
    blob = self._lines[start]
    expansions = 0
    i = start + 1
    while i < len(self._lines):
      line = self._lines[i]
      if not re.match(_match_all_whitespace, line):
        spaces = re.match(_capture_leading_spaces, line)
        indent = len(spaces.group(0))
        if indent <= base_indent:
          break
        expansions += 1
        blob += line
      i += 1
    if expansions == 0:
      msg = 'No RHS for ' + symbol + ' at ' + fname + ':' + str(start + 1)
      raise Exception(msg)
    self._productions.append((symbol, start, blob))
    return i

  def _parse_syntax_block(self, start):
    line = self._lines[start]
    assert(re.match(_match_syntax_begin, line))
    vprint('Syntax begin at', self._fname, ':', (start + 1))
    i = start + 1
    while i < len(self._lines):
      line = self._lines[i]
      if re.match(_match_syntax_end, line):
        vprint('Syntax end at', self._fname, ':', (i + 1))
        break
      if not re.match(_match_all_whitespace, line):
        production = re.match(_capture_production, line)
        if production:
          i = self._parse_production_blob(i)
          continue
        msg = 'Garbled syntax block at ' + self._fname + ':' + str(i + 1)
        raise Exception(msg)
      i += 1
    return i

  def _collect_productions(self):
    i = 0
    while i < len(self._lines):
      line = self._lines[i]
      if re.match(_match_syntax_begin, line):
        i = self._parse_syntax_block(i)
        continue
      i += 1

  def get_productions(self):
    return self._productions


def get_all_files_to_process():
  result = []
  for rstfile in _rstfiles:
    if os.path.isfile(rstfile):
      # Skip the file we're producing to avoid recursion.
      if not os.path.basename(rstfile) == 'syntax.rst':
        result.append(rstfile)
    elif os.path.isdir(rstfile):
      for dirpath, dirnames, filenames, in os.walk(rstfile):
        for f in filenames:
          if not f.endswith('.rst'):
            continue
          path = os.path.join(dirpath, f)
          if os.path.basename(path) == 'syntax.rst':
            continue
          result.append(path)
    else:
      msg = 'File or directory not found: ' + str(rstfile)
      raise Exception(msg)
  return result 


def error_if_duplicate_rule(rstfile, ident, lineno):
  if ident in _productions:
    fname, lineno, _ = _productions[ident]
    msg = 'In ' + str(rstfile) + ', line ' + str(lineno) + '\n'
    msg += 'Duplicate identifier for rule: ' + str(ident) + '\n'
    msg += 'First defined in ' + str(fname) + ' line ' + str(lineno) + '\n'
    vprint(msg)
    raise Exception(msg)


def collect_productions_from_files():
  fnames = get_all_files_to_process()
  for fname in fnames:
    vprint('Processing file:', fname)
    parser = SyntaxParser(fname)
    collected = parser.get_productions()
    vprint('Captured', len(collected), 'productions in', fname)
    for ident, lineno, blob in collected:
      global _productions
      error_if_duplicate_rule(fname, ident, lineno)
      _productions[ident] = (fname, lineno, blob)


# Use a triple escaped string to neatly capture prelude text blob.
_prelude_text = """.. default-domain:: chpl

.. _Chapter-Syntax:

Syntax
======

This appendix collects the syntax productions listed throughout the
language specification. There are no new syntax productions listed
in this appendix. The productions are listed both alphabetically and
in application order for convenience.

:ref:`Productions_Listed_Alphabetical_Order`

:ref:`Productions_Listed_Application_Order`

"""


def output_prelude(out):
  out.write(_prelude_text)


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


def is_meta_keyword(ident):
  return ident == 'OPT' or ident == '-' or ident == '_' or ident == 'b'


def is_meta_sentence(line):
  return line.lstrip().startswith('any character except')


def get_nonterminals(line):
  # Regex trick, third capture group contains valid nonterminals.
  regex_groups = re.findall(_capture_nonterminal, line)
  result = []
  for group in regex_groups:
    quoted, escaped, ok = group
    # TODO: Use some regex-fu to eliminate this special case?
    # ''' uninterpreted-single-quote-delimited-characters '''
    # This odd nonterminal appears in 'uninterpreted-string-literal' and is
    # triple quoted. Instead of handling it with a fourth regex group in
    # _capture_nonterminal, special case it here.
    if quoted:
      clean = quoted.lstrip('\' ').rstrip('\' ')
      if clean == 'uninterpreted-single-quote-delimited-characters':
        result.append(clean)
    if ok:
      result.append(ok)
  return result

  
def get_nonterminals_in_expansion(ident, stack, applied):
  global _productions
  fname, lineno, blob = _productions[ident]
  vprint('Inspecting expansions of:', ident)
  vprint(blob)
  nonterminals = []
  for line in blob.split('\n'):
    if re.match(_capture_production, line) or is_meta_sentence(line):
      continue
    nonterminals += get_nonterminals(line)
  vprint('Found', len(nonterminals), 'nonterminals')
  vprint(nonterminals)
  vprint()
  for n in nonterminals:
    if is_meta_keyword(n):
      continue
    if not n in _productions:
      msg = 'Nonterminal not in production list: ' + str(n)
      raise Exception(msg)
    if not n in applied:
      stack.append(n)
      applied.add(n)


def output_productions_application_order(out):
  out.write('.. _Productions_Listed_Application_Order:\n')
  out.write('\n')
  title = 'Productions Listed in Application Order'
  out.write(title + '\n')
  out.write((len(title) * '-') + '\n')
  out.write('\n')
  out.write('.. code-block:: syntax\n')
  out.write('\n')
  global _productions, _start
  vprint('Computing application-first ordering starting with:', _start)
  stack = [_start]
  applied = {_start}
  # Do a breadth-first search over all productions...
  while stack:
    ident = stack.pop(0)
    get_nonterminals_in_expansion(ident, stack, applied)
    _, _, blob = _productions[ident]
    out.write(blob)
    out.write('\n')
  # Consider any unapplied productions to be a fatal error...
  unapplied_count = len(_productions) - len(applied)
  if unapplied_count != 0:
    msg = str(unapplied_count) + ' nonterminals are never applied'
    vprint(msg)
    for ident in _productions.keys():
      if not ident in applied:
        vprint(ident)
    raise Exception(msg)


def main():
  args = vars(get_script_arguments())
  global _rstfiles, _outdir, _verbose
  _rstfiles = args['rstfiles']
  _outdir = args['outdir']
  _verbose = args['verbose']
  collect_productions_from_files()
  outname = os.path.join(_outdir, 'syntax.rst')
  vprint('Writing output to:', outname)  
  with open(outname, 'w') as outfile:
    output_prelude(outfile)
    output_productions_alphabetical(outfile)
    output_productions_application_order(outfile)


if __name__ == '__main__':
  main()

