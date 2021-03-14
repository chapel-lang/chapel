#!/usr/bin/env python3
# encoding: utf-8

"""Scipy.sparse CSR * CSC matrix-matrix multiplication"""

import sys

import scipy.sparse as sparse
import numpy as np

import time

def main(m=1000, n=1000, sparsity=0.01):
    A = sparse.random(m, n, density=sparsity, format='csr', dtype=np.float64) # float64?
    B = sparse.random(n, m, density=sparsity, format='csc', dtype=np.float64) # float64?

    print(A.getnnz())
    print(B.getnnz())
    start = time.time()
    C = A.dot(B)
    C.sort_indices()
    stop = time.time()
    print('Python: ', stop-start)
    print(C.getnnz())
    print(bool(C.has_sorted_indices))




if __name__ == '__main__':

    # Arg-parsing
    if len(sys.argv) == 1:
        main()
    elif len(sys.argv) == 2:
        m = int(sys.argv[1])
        main(m, m)
    elif len(sys.argv) == 3:
        m = int(sys.argv[1])
        n = int(sys.argv[2])
        main(m, n)
    else:
        m = int(sys.argv[1])
        n = int(sys.argv[2])
        s = float(sys.argv[3])
        main(m, n, s)


