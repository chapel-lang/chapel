
def strNested(arr, N, d=1):
    if N == 1:
        return '[' + ', '.join(str(x) for x in arr) + ']'
    else:
        return '[' + f'\n{" " * d}'.join([strNested(a, N - 1, d + 1) for a in arr]) + ']'
