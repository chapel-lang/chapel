module TwoArrayDistributedSampleSort {
  private use Sort;
  private use TwoArrayPartitioning;
  private use SampleSortHelp;
  private use RadixSortHelp;
  private use TwoArraySampleSort;
  private use TwoArrayDistributedPartitioning;

  private use CTypes;

  proc twoArrayDistributedSampleSort(ref Data:[],
                                     comparator:?rec = new DefaultComparator()) {

    // just run the local version if Data isn't distributed
    if Data._instance.isDefaultRectangular() {
      twoArraySampleSort(Data, comparator);
      return;
    }

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
      bucketizerType=SampleBucketizer(Data.eltType),
      numLocales=Data.targetLocales().size,
      baseCaseSize=baseCaseSize,
      distributedBaseCaseSize=distributedBaseCaseSize,
      endbit=endbit);
    var state2 = new TwoArrayDistributedBucketizerSharedState(
      bucketizerType=SampleBucketizer(Data.eltType),
      numLocales=Data.targetLocales().size,
      baseCaseSize=baseCaseSize,
      distributedBaseCaseSize=distributedBaseCaseSize,
      endbit=endbit);

    distributedPartitioningSortWithScratchSpace(
                                     Data.domain.low.safeCast(int),
                                     Data.domain.high.safeCast(int),
                                     Data, Scratch,
                                     state1, state2, comparator, 0);

    _do_destroy_array(Scratch, deinitElts=false);
  }
}
