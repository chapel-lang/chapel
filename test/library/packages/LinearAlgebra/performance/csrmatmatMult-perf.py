#!/usr/bin/env python3
# encoding: utf-8

"""
This script is used for manual validation.
"""

from __future__ import print_function

import numpy as np
from scipy import sparse

import os
import time

def read_csr_matrix_file(f):
    data = []
    row_ind = []
    col_ind = []
    with open(f, 'r') as handle:
        m = int(handle.readline())
        n = int(handle.readline())
        for line in handle.readlines():
            try:
                elements = line.split()
                value = np.float64(elements[0])
                row_ind.append(int(elements[1])-1)
                col_ind.append(int(elements[2])-1)
                data.append(value)
            except:
                print('Error in parsing line:')
                print(line)
                raise()
    A = sparse.csr_matrix((data, (row_ind, col_ind)), shape=(m, n), dtype=np.float64)
    return A

def write_csr_matrix_file(f, A):
    m,n = A.shape
    with open(f, 'w') as handle:
        print(m, file=handle)
        print(n, file=handle)
        rows, cols = A.nonzero()
        for i,j in zip(rows, cols):
            print('{} {} {}'.format(A[i,j], i+1, j+1), file=handle)

if not os.path.isfile('A.txt') or not os.path.isfile('AA.txt'):
    print('This program requires A.txt and AA.txt to be generated from Chapel')
    exit()

A = read_csr_matrix_file('A.txt')
write_csr_matrix_file('Apy.txt', A) # sanity check
start = time.time()
AA = A.dot(A)
end = time.time()
print('python time (s): ', end - start)

#print(AA.indptr)
#print(AA.indices)
#print(AA.has_sorted_indices)
#print(AA.sort_indices())
#print(AA.indptr)
#print(AA.indices)


# Validate results
AAref = read_csr_matrix_file('AA.txt')
AA.sort_indices()

if (AAref!=AA).nnz != 0:
    print('Python A*A does not match AA.txt')

write_csr_matrix_file('AApy.txt', AA)

#print(np.allclose(AAref.A, A2.A))
