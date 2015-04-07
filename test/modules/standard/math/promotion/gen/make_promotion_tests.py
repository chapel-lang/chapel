#!/usr/bin/env python
# Create promotion tests
def write_test(name, min, max):
    filename = '../' + name + '.chpl';
    with open(filename, 'w') as fout:
        fout.write('// ' + filename + '\n');
        fout.write('// \n');
        fout.write('// AUTO-GENERATED, DO NOT EDIT.  See gen/ directory.\n');
        fout.write('// \n');
        fout.write('config param n = 10;\n');
        fout.write('var D = {0..n};\n');
        fout.write('var A : [D] real;\n');
        fout.write('var B : [D] real;\n');
        fout.write('var s = (' + max + ' - ' + min + ') / n;\n');
        fout.write('var m = ' + min + ';\n');
        fout.write('A = [i in D] s * i + m;\n');
        fout.write('B = ' + name + '(A);\n');
        fout.write('writeln(A);\n');
        fout.write('writeln(B);\n');

with open("monadic.txt") as fin:
    while True:
        line = fin.readline();
        if len(line) == 0:
            break;
        name, min, max = line.rstrip().split(' ');
        write_test(name, min, max)
