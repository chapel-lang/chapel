#!/usr/bin/env python
# Create promotion tests
import os

monadic = [('abs', -10.0, 10.0),
           ('acos', -1.0, 1.0),
           ('acosh', 1.0, 10.0),
           ('asin', -1.0, 1.0),
           ('asinh', -1.0, 1.0),
           ('atan', -10.0, 10.0),
           ('atanh', -1.0, 1.0),
           ('cbrt', 0.0, 1000.0),
           ('ceil', -7.0, 7.0),
           ('cos', -7.0, 7.0),
           ('cosh', -7.0, 7.0),
           ('erf', 0.0, 1.0),
           ('erfc', 0.0, 1.0),
           ('exp', 0.0, 3.0),
           ('exp2', 0.0, 10.0),
           ('expm1', 0.0, 10.0),
           ('floor', -7.0, 7.0),
           ('lgamma', -6.5, 7.0),
           ('log', 1.0, 10.0),
           ('log10', 1.0, 1000.0),
           ('log1p', 1.0, 10.0),
           ('log2', 1.0, 1024.0),
           ('nearbyint', -7.0, 7.0),
           ('rint', -7.0, 7.0),
           ('round', -7.0, 7.0),
           ('sgn', -7.0, 7.0),
           ('sin', -7.0, 7.0),
           ('sinh', -7.0, 7.0),
           ('sqrt', 0.0, 100.0),
           ('tan', -7.0, 7.0),
           ('tanh', -3.0, 3.0),
           ('tgamma', -6.5, 7.0),
           ('trunc', -7.0, 7.0)]

def write_test(name, fmin, fmax):
    min = str(fmin)
    max = str(fmax)
    filename = name + '.chpl'
    this_dir = os.path.abspath(os.path.dirname(__file__))
    (test_dir, _) = os.path.split(this_dir)
    path = os.path.join(test_dir, filename)
    print path
    with open(path, 'w') as fout:
        fout.write('// ' + filename + '\n')
        fout.write('// \n')
        fout.write('// AUTO-GENERATED, DO NOT EDIT.  See generate_tests/ directory.\n')
        fout.write('// \n\n')
        fout.write('proc roundIfClose(x: real) {\n')
        fout.write('  const eps = 1e-8;\n')
        fout.write('  if abs(x) < eps then return 0.0; // special case, to avoid -0.0\n')
        fout.write('  if abs(round(x) - x) < eps then return round(x);\n')
        fout.write('  return x;\n')
        fout.write('}\n\n')
        fout.write('config param n = 10;\n')
        fout.write('var D = {0..n};\n')
        fout.write('var A : [D] real;\n')
        fout.write('var B : [D] real;\n')
        fout.write('var s = (' + max + ' - ' + min + ') / n;\n')
        fout.write('var m = ' + min + ';\n')
        fout.write('A = [i in D] s * i + m;\n')
        fout.write('A = roundIfClose(A);\n')
        fout.write('B = ' + name + '(A);\n')
        fout.write('A = roundIfClose(A);\n')
        fout.write('B = roundIfClose(B);\n')
        fout.write('writeln(A);\n')
        fout.write('writeln(B);\n')

for (name, min, max) in monadic:
    write_test(name, min, max)
