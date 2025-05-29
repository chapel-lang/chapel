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



// shell sort is in-place but not stable
@chpldoc.nodoc
module ShellSort {
  private use Sort;
  proc shellSort(ref Data: [?Dom] ?eltType,
                 comparator:?rec = new defaultComparator(), start = Dom.low,
                 end = Dom.high)
  {
    chpl_check_comparator(comparator, eltType);

    type idxType = Data.idxType;

    if Dom.rank != 1 then
      compilerError("shellSort() requires 1-D array");
    if ! Dom.hasUnitStride() then
      compilerError("shellSort() requires an array over a non-stridable domain");

    // Based on Sedgewick's Shell Sort -- see
    // Analysis of Shellsort and Related Algorithms 1996
    // and see Marcin Ciura - Best Increments for the Average Case of Shellsort
    // for the choice of these increments.
    var js,hs:idxType;
    var v:Data.eltType;
    const incs = (701, 301, 132, 57, 23, 10, 4, 1);
    for hh in incs {
      // skip past cases in which the 'incs' value was too big for
      // idxType, or in which h+start will overflow idxType.
      // start may be negative, so the first test isn't redundant.
      if hh > max(idxType) || hh >= max(idxType):uint - start:uint then
        continue;

      const h = hh:idxType;
      hs = h + start;
      for is in hs..end {
        v = Data[is];
        js = is;
        while js >= hs && chpl_compare(v,Data[js-h],comparator) < 0 {
          Data[js] = Data[js - h];
          js -= h;
        }
        Data[js] = v;
      }
    }
  }

  // Like the shell sort above, but this version uses
  // ShallowCopy to move elements instead of assigning them.
  proc shellSortMoveElts(ref Data: [?Dom] ?eltType,
                         comparator:?rec = new defaultComparator(),
                         start: Data.idxType = Dom.low,
                         end: Data.idxType = Dom.high)
  {
    chpl_check_comparator(comparator, eltType);

    type idxType = Data.idxType;

    if Dom.rank != 1 then
      compilerError("shellSort() requires 1-D array");
    if ! Dom.hasUnitStride() then
      compilerError("shellSort() requires an array over a non-stridable domain");

    // Based on Sedgewick's Shell Sort -- see
    // Analysis of Shellsort and Related Algorithms 1996
    // and see Marcin Ciura - Best Increments for the Average Case of Shellsort
    // for the choice of these increments.
    var js,hs:idxType;
    const incs = (701, 301, 132, 57, 23, 10, 4, 1);
    for hh in incs {
      // skip past cases in which the 'incs' value was too big for
      // idxType, or in which h+start will overflow idxType.
      // start may be negative, so the first test isn't redundant.
      if hh > max(idxType) || hh >= max(idxType):uint - start:uint then
        continue;

      const h = hh:idxType;
      hs = h + start;
      for is in hs..end {
        // move Data[is] into v
        pragma "no auto destroy"
        pragma "no copy"
        var v = ShallowCopy.shallowCopyInit(Data[is]);
        js = is;
        while js >= hs && chpl_compare(v,Data[js-h],comparator) < 0 {
          // move Data[js - h] into Data[js]
          ShallowCopy.shallowCopy(Data[js], Data[js - h]);
          js -= h;
        }
        // move v into Data[js]
        ShallowCopy.shallowCopy(Data[js], v);
      }
    }
  }

  // this version takes int start and end and casts to Data.idxType
  // to make it more convenient to call from the radix sorting code
  proc shellSortMoveEltsIntIdx(ref Data: [?Dom] ?eltType,
                               comparator:?rec = new defaultComparator(),
                               start:int = Dom.low,
                               end:int = Dom.high)
  {
    type idxType = Data.idxType;
    shellSortMoveElts(Data, comparator, start:idxType, end:idxType);
  }
}


