/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



// this quick sort is not stable
// it is in-place however
// it is parallel but has limited parallelism
@chpldoc.nodoc
module QuickSort {
  private use Sort;
  use Sort.ShallowCopy;

  /*
   Partition the array Data[lo..hi] using the pivot at Data[pivIdx].

   This is the 3-way symmetric partition described
   in Engineering a Sort Function (1993) by Jon L. Bentley , M. Douglas McIlroy

   Returns the (eqStart,eqEnd) where eqStart..eqEnd elements are
   equal to the pivot (and elements less are before eqStart and elements
   greater are after eqEnd).
   */
  proc partition(ref Data: [?Dom] ?eltType,
                 lo: int, pivIdx: int, hi: int,
                 comparator)
  {
    // The following section categorizes array elements as follows:
    //
    //   |  =  |  <  |  ?  |  >  |  =   |
    //    lo    a     b   c     d     hi
    //
    //  lo..a-1 stores equal elements
    //   a..b-1 stores elements < pivot
    //   b..c   store uncategorized elements
    // c+1..d-1 store elements > pivot
    // d+1..hi  stores equal elements

    // initially, entire array is in b..c
    var a = lo;
    var b = lo;
    var c = hi;
    var d = hi;

    // Now put the pivot in Data[lo] so we can
    // avoid keeping track of its position.
    if lo != pivIdx then
      ShallowCopy.shallowSwap(Data[lo], Data[pivIdx]);

    a += 1;
    b += 1;

    // Now swap the pivot to a local variable
    pragma "no auto destroy"
    pragma "no copy"
    var piv: eltType = ShallowCopy.shallowCopyInit(Data[lo]); // leaves Data[lo] empty

    while true {
      while b <= c {
        // continue while Data[b] <= piv
        var cmp = chpl_compare(Data[b], piv, comparator);
        if cmp > 0 then
          break;
        if cmp == 0 {
          if a != b {
            ShallowCopy.shallowSwap(Data[a], Data[b]);
          }
          a += 1; // one more equal element (on left)
        }
        b += 1; // one more categorized element
      }
      while c >= b {
        // continue while Data[c] >= piv
        var cmp = chpl_compare(Data[c], piv, comparator);
        if cmp < 0 then
          break;
        if cmp == 0 {
          ShallowCopy.shallowSwap(Data[d], Data[c]);
          d -= 1; // one more equal element (on right)
        }
        c -= 1; // one more categorized element
      }
      if b > c then
        break; // stop here

      // then Data[b] > piv and Data[c] < piv,
      // so Data[c] < Data[b] and they are an inversion
      ShallowCopy.shallowSwap(Data[b], Data[c]);
      b += 1;
      c -= 1;
    }

    // Now put piv back in Data[lo]
    ShallowCopy.shallowCopy(Data[lo], piv); // leaves piv empty

    // now we are in the state:
    //   |  =  |  <  |  > |  =   |
    //    lo    a   c b  d     hi

    // now place the equal regions in the right places
    var s, l, h: int;

    // Fix the first = region
    s = min(a-lo, b-a); // the number of of elements to swap
    l = lo;
    h = b-s;
    while s > 0 {
      ShallowCopy.shallowSwap(Data[l], Data[h]);
      l += 1;
      h += 1;
      s -= 1;
    }

    // Fix the second = region
    var n = hi+1;
    s = min(d-c, hi-d);
    l = b;
    h = n-s;
    while s > 0 {
      ShallowCopy.shallowSwap(Data[l], Data[h]);
      l += 1;
      h += 1;
      s -= 1;
    }

    var eqStart = b-a+lo;
    var eqEnd = hi-(d-c);

    return (eqStart, eqEnd);
  }


  // Returns the index of the median element
  // of Data[lo], Data[mid], Data[hi]
  // (in other words it returns lo, mid, or hi).
  proc order3(Data: [?Dom] ?eltType,
              lo: int, mid: int, hi: int,
              comparator): int {

    if chpl_compare(Data[lo], Data[mid], comparator) < 0 {
      // lo < mid
      if chpl_compare(Data[hi], Data[lo], comparator) < 0 {
        // lo < mid, hi < lo -> hi < lo < mid
        return lo;
      } else if chpl_compare(Data[mid], Data[hi], comparator) < 0 {
        // lo < mid, lo <= hi, mid < hi -> lo < mid < hi
        return mid;
      } else {
        // lo < mid, lo <= hi, hi <= mid -> lo <= hi <= mid
        return hi;
      }
    } else {
      // mid <= lo
      if chpl_compare(Data[lo], Data[hi], comparator) < 0 {
        // mid <= lo, lo < hi -> mid <= lo < hi
        return lo;
      } else if chpl_compare(Data[hi], Data[mid], comparator) < 0 {
        // mid <= lo, hi <= lo, hi < mid -> hi < mid <= lo
        return mid;
      } else {
        // mid <= lo, hi <= lo, mid <= hi -> mid <= hi <= lo
        return hi;
      }
    }
  }

 /* Use quickSort to sort Data */
 proc quickSort(ref Data: [?Dom] ?eltType,
                minlen = 16,
                comparator:?rec = new defaultComparator(),
                region:range(?) = Data.domain.dim(0)) {

    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("quickSort() requires 1-D array");
    }

    if region.strides != strideKind.one && region.stride != 1 {
      ref reindexed = Data.reindex(region.low..#region.size);
      assert(reindexed.domain.stride == 1);
      quickSortImpl(reindexed, minlen, comparator);
      return;
    }

    assert(Dom.stride == 1);
    quickSortImpl(Data, minlen, comparator,
                  start=region.low, end=region.high);
  }


  /* Non-stridable quickSort to sort Data[start..end] */
  proc quickSortImpl(ref Data: [?Dom] ?eltType,
                     minlen=16,
                     comparator:?rec = new defaultComparator(),
                     start:int = Dom.low, end:int = Dom.high) {
    import Sort.InsertionSort;

    // grab obvious indices
    var lo = start,
        hi = end;

    // keep iterating over subproblems
    while lo < hi {
        var mid = lo + (hi - lo + 1) / 2;
        var piv = mid;

        if hi - lo < minlen {
          // base case -- use insertion sort
          InsertionSort.insertionSortMoveElts(Data, comparator=comparator, lo, hi);
          return;
        } else if hi <= lo {
          // nothing to sort
          return;
        }

        // find pivot using median-of-3 method for small arrays
        // and a "ninther" for bigger arrays. Places the pivot in
        // Data[lo].
        if hi - lo < 100 {
          piv = order3(Data, lo, mid, hi, comparator);
        } else {
          // assumes array size > 9 at the very least

          // median of each group of 3
          const medLo  = order3(Data, lo,    lo+1, lo+2,  comparator);
          const medMid = order3(Data, mid-1, mid,  mid+1, comparator);
          const medHi  = order3(Data, hi-2,  hi-1, hi,    comparator);
          // median of the medians
          piv = order3(Data, medLo, medMid, medHi, comparator);
        }

        var (eqStart, eqEnd) = partition(Data, lo, piv, hi, comparator);

        // stay sequential
        if hi-lo < 300 || here.runningTasks() > here.numPUs(logical=true)  {
          // handle smaller subproblem recursively and iterate for larger one
          if eqStart - lo > hi - eqEnd {
            // recur for smaller right half
            quickSortImpl(Data, minlen, comparator, eqEnd + 1, hi);
            // change value of hi to the end of left half
            hi = eqStart - 1;
          } else {
            // recur for smaller left half
            quickSortImpl(Data, minlen, comparator, lo,        eqStart - 1);
            // change value of lo to the start of right half
            lo = eqEnd  + 1;
          }
        } else {
          // do the subproblems in parallel
          cobegin {
            quickSortImpl(Data, minlen, comparator, lo, eqStart-1);
            quickSortImpl(Data, minlen, comparator, eqEnd+1, hi);
          }
          break;
        }
      }
    }
}

