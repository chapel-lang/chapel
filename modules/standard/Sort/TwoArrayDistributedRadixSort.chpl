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
module TwoArrayDistributedRadixSort {
  import Sort.defaultComparator;
  private use super.TwoArrayDistributedPartitioning;
  private use super.RadixSortHelp;
  private use super.TwoArrayRadixSort;

  proc twoArrayDistributedRadixSort(ref Data:[],
                                    comparator:?rec = new defaultComparator()) {

    // just run the local version if Data isn't distributed
    if Data._instance.isDefaultRectangular() {
      twoArrayRadixSort(Data, comparator);
      return;
    }

    var sequentialSizePerTask=4096;
    var baseCaseSize=16;
    var distributedBaseCaseSize=1024;

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    pragma "no auto destroy"
    var Scratch: Data.type =
      Data.domain.buildArray(Data.eltType, initElts=false);

    // TODO: do some first-touch, which should matter for comm=ugni
    Scratch.dsiElementInitializationComplete();

    var state1 = new TwoArrayDistributedBucketizerSharedState(
      bucketizerType=RadixBucketizer,
      numLocales=Data.targetLocales().size,
      baseCaseSize=baseCaseSize,
      distributedBaseCaseSize=distributedBaseCaseSize,
      endbit=endbit);
    var state2 = new TwoArrayDistributedBucketizerSharedState(
      bucketizerType=RadixBucketizer,
      numLocales=Data.targetLocales().size,
      baseCaseSize=baseCaseSize,
      distributedBaseCaseSize=distributedBaseCaseSize,
      endbit=endbit);

    distributedPartitioningSortWithScratchSpace(
                                     Data.domain.low.safeCast(int),
                                     Data.domain.high.safeCast(int),
                                     Data, Scratch,
                                     state1, state2,
                                     comparator, 0);

    _do_destroy_array(Scratch, deinitElts=false);
  }
}

