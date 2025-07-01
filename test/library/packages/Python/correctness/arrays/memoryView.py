import io


def dump_array(arr):
    m = memoryview(arr)
    f = io.StringIO()
    print(arr, file=f)
    print("  format:", m.format, file=f)
    print("  itemsize:", m.itemsize, file=f)
    print("  shape:", m.shape, file=f)
    print("  strides:", m.strides, file=f)
    print("  ndim:", m.ndim, file=f)
    print("  nbytes:", m.nbytes, file=f)
    print("  readonly:", m.readonly, file=f)
    print("  c_contiguous:", m.c_contiguous, file=f)
    return f.getvalue()
