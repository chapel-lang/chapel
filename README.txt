The branch `rd2` is my experiment of making ranges undefaulted generics.

Also introducing:

    type boundedRange = range(bounds = boundedKind.both, ?);
    type simpleRange = range(int, boundedKind.both, strideKind.unit);

