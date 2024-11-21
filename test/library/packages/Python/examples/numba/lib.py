import numba

@numba.cfunc(numba.int64(numba.int64))
def apply(x):
    return x + 1 if x % 2 != 0 else x
