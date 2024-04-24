#
# Copyright 2024-2024 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from typing import Callable, Sequence, Optional


# reimplement bisect with a key, since the parameter wasn't added until python 3.10+
def bisect_right(a: Sequence, x, key: Optional[Callable] = None):
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


def bisect_left(a: Sequence, x, key: Optional[Callable] = None):
    lo = 0
    hi = len(a)

    if key is None:
        key = lambda e: e

    while lo < hi:
        mid = (lo + hi) // 2
        if x <= key(a[mid]):
            hi = mid
        else:
            lo = mid + 1
    return lo
