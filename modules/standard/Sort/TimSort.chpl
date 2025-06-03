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



// TimSort is stable, nlogn, but not in-place
@chpldoc.nodoc
module TimSort {
  private use Sort;

    /*
    Sort the 1D array `x` using a parallel timSort algorithm.
    For more information on timSort, follow the link.
      https://github.com/python/cpython/blob/master/Objects/listsort.txt

    :arg x: The array to be sorted
    :type x: [] `eltType`
    :arg blockSize: use :proc: `insertionSort` on blocks of `blockSize`
    :type blockSize: `integral`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */

  proc timSort(ref x: [],
               blockSize = 16,
               comparator: ? = new defaultComparator(),
               region: range(?) = x.dim(0))
  {
    chpl_check_comparator(comparator, x.eltType);

    if x.rank != 1 {
      compilerError("timSort() requires 1-D array");
    }

    _TimSort(x, region.low, region.high, blockSize, comparator);
  }

  private proc _TimSort(ref Data: [?Dom], lo:int, hi:int, blockSize=16,
                        comparator:?rec = new defaultComparator()) {
    import Sort.InsertionSort;

    /*Parallelly apply insertionSort on each block of size `blockSize`
     using forall loop*/

    const stride = abs(Dom.stride): Dom.idxType;
    const size = (hi - lo) / stride + 1;
    const chunks = (size + blockSize - 1) / blockSize;

    forall i in 0..#chunks with (ref Data) {
      InsertionSort.insertionSort(Data, comparator = comparator, lo + (i * blockSize) * stride, min(hi, lo + ((i + 1) * blockSize * stride) - stride));
    }

    /* apply merge operations on each block
    *as the merges at a level are independent of each other
    *they can be applied in parallel
    */

    var numSize = blockSize;
    while(numSize < size) {
      forall i in 0..<size by 2 * numSize with (ref Data) {

        const l = lo + i * stride;
        const mid = lo + (i + numSize - 1) * stride;
        const r = min(lo + (i + 2 * numSize - 1) * stride, hi);

        _Merge(Data, l, mid, r, comparator=comparator);
      }

      numSize = numSize * 2;

    }
  }

  /*
   This TimSort._Merge() differs from MergeSort._Merge() in the following way:
   MergeSort._Merge() alternates the storage of segments in the original memory and the copied memory.
   TimSort._Merge() creates a copy of the segments to be merged and
   stores the results back into the original memory.
  */
  private proc _Merge(ref Dst: [?Dom] ?eltType, lo:int, mid:int, hi:int,
                      comparator:?rec = new defaultComparator()) {
    /* Data[lo..mid by stride] is much slower than Data[lo..mid] when
     * Dom is unstrided.  So specify the latter explicitly when possible. */
    if mid >= hi {
      return;
    }
    const stride = abs(Dom.stride): Dom.idxType;
    const a1range = if Dom.hasPosNegUnitStride() then lo..mid
                    else lo..mid by stride:uint;
    const a1max = mid;

    const a2range = if Dom.hasPosNegUnitStride() then (mid+1)..hi
                    else (mid+stride)..hi by stride:uint;
    const a2max = hi;

    var A1 = Dst[a1range];
    var A2 = Dst[a2range];

    var a1 = a1range.first;
    var a2 = a2range.first;
    var i = lo;
    while ((a1 <= a1max) && (a2 <= a2max)) {
      if (chpl_compare(A1(a1), A2(a2), comparator) <= 0) {
        Dst[i] = A1[a1];
        a1 += stride;
        i += stride;
      } else {
        Dst[i] = A2[a2];
        a2 += stride;
        i += stride;
      }
    }
    while (a1 <= a1max) {
      Dst[i] = A1[a1];
      a1 += stride;
      i += stride;
    }
    while (a2 <= a2max) {
      Dst[i] = A2[a2];
      a2 += stride;
      i += stride;
    }
  }
}

