
from typing import Callable, Sequence, Optional

# reimplement bisect with a key, since the parameter wasn't added until python 3.10+
def bisect_right(a: Sequence, x, key: Optional[Callable]=None):
    lo = 0
    hi = len(a)

    if key is None:
        key = lambda e: e

    while lo < hi:
        mid = (lo + hi) // 2
        if x < key(a[mid]):
            hi = mid
        else:
            lo = mid + 1
    return lo
