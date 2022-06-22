#!/usr/bin/python3

import sys

def main():
  hi = int(sys.argv[1])
  assert(hi > 0)
  nl = '\n'
  with open('superbigif.chpl', 'w+') as fp:
    fp.write('// Testing max parse depth for branches' + nl)
    fp.write('// Depth: {}'.format(hi) + nl)
    fp.write('const depth = {};'.format(hi) + nl)
    for i in range(0, hi+1):
      if i == 0:
        fp.write('if depth == {} {{'.format(i) + nl)
        fp.write('  writeln({});'.format(i) + nl)
        fp.write('}')
      else:
        fp.write(' else if depth == {} {{'.format(i) + nl)
        fp.write('  writeln({});'.format(i) + nl)
        fp.write('}')
    fp.write(' else {' + nl)
    fp.write('  writeln(\'Hmmm...\');' + nl)
    fp.write('}' + nl)
  with open('superbigif.good', 'w+') as fp:
    fp.write('{}'.format(hi) + nl)
  return

if __name__ == '__main__':
  main()

