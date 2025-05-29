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




// insertion sort is stable and in-place but has
// poor time complexity (O(n**2)) for large problem sizes
@chpldoc.nodoc
module InsertionSort {
  private use Sort;
  /*
   Sort the 1D array `Data` in-place using a sequential insertion sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc insertionSort(ref Data: [?Dom] ?eltType,
                     comparator:?rec = new defaultComparator(), lo:int=Dom.low,
                     hi:int=Dom.high) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("insertionSort() requires 1-D array");
    }

    const low = lo,
          high = hi,
          stride = abs(Dom.stride);

    for i in low..high by stride {
      var ithVal = Data[i];
      var inserted = false;
      for j in low..i-stride by -stride {
        if chpl_compare(ithVal, Data[j], comparator) < 0 {
          Data[j+stride] = Data[j];
        } else {
          Data[j+stride] = ithVal;
          inserted = true;
          break;
        }
      }
      if (!inserted) {
        Data[low] = ithVal;
      }
    }
  }

  proc insertionSortMoveElts(ref Data: [?Dom] ?eltType,
                             comparator:?rec = new defaultComparator(),
                             lo:int=Dom.low, hi:int=Dom.high) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("insertionSort() requires 1-D array");
    }

    const low = lo,
          high = hi,
          stride = abs(Dom.stride);

    for i in low..high by stride {
      pragma "no auto destroy"
      pragma "no copy"
      var ithVal = ShallowCopy.shallowCopyInit(Data[i]);

      var inserted = false;
      for j in low..i-stride by -stride {
        if chpl_compare(ithVal, Data[j], comparator) < 0 {
          ShallowCopy.shallowCopy(Data[j+stride], Data[j]);
        } else {
          ShallowCopy.shallowCopy(Data[j+stride], ithVal);
          inserted = true;
          break;
        }
      }
      if (!inserted) {
        ShallowCopy.shallowCopy(Data[low], ithVal);
      }
    }
  }
}

