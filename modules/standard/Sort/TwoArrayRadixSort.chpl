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



@chpldoc.nodoc
module TwoArrayRadixSort {
  import Sort.defaultComparator;
  private use super.TwoArrayPartitioning;
  private use super.RadixSortHelp;

  proc twoArrayRadixSort(ref Data:[],
                         comparator:?rec = new defaultComparator(),
                         region:range(?) = Data.domain.dim(0)) {

    if !chpl_domainDistIsLayout(Data.domain) {
      compilerWarning("twoArrayRadix sort no longer handles distributed arrays. Please use TwoArrayDistributedRadixSort.twoArrayDistributedRadixSort instead (but note that it is not stable)");
    }

    var baseCaseSize=16;
    var sequentialSizePerTask=4096;
    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    var ScratchDom = {region};
    pragma "no auto destroy"
    var Scratch: [ScratchDom] Data.eltType =
      ScratchDom.buildArray(Data.eltType, initElts=false);

    // It would make sense to touch the memory first here, but early experiments
    // suggest that it doesn't help with CHPL_COMM=none.
    Scratch.dsiElementInitializationComplete();

    var state = new TwoArrayBucketizerSharedState(
      bucketizer=new RadixBucketizer(),
      baseCaseSize=baseCaseSize,
      sequentialSizePerTask=sequentialSizePerTask,
      endbit=endbit);


    partitioningSortWithScratchSpace(region.low.safeCast(int),
                                     region.high.safeCast(int),
                                     Data, Scratch,
                                     state, comparator, 0);

    _do_destroy_array(Scratch, deinitElts=false);
  }
}


