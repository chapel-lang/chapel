module TwoArraySampleSort {
  private use Sort;
  private use TwoArrayPartitioning;
  private use SampleSortHelp;
  private use RadixSortHelp;

  private use CTypes;

  proc twoArraySampleSort(ref Data:[],
                          comparator:?rec = new DefaultComparator()) {

    var baseCaseSize=16;
    var distributedBaseCaseSize=1024;

    // Allocate the Scratch array.
    pragma "no auto destroy"
    var Scratch: Data.type =
      Data.domain.buildArray(Data.eltType, initElts=false);

    // It would make sense to touch the memory first here, but early experiments
    // suggest that it doesn't help with CHPL_COMM=none.
    Scratch.dsiElementInitializationComplete();

    var state = new TwoArrayBucketizerSharedState(
      bucketizer=new SampleBucketizer(Data.eltType),
      baseCaseSize=baseCaseSize);

    partitioningSortWithScratchSpace(Data.domain.low.safeCast(int),
                                     Data.domain.high.safeCast(int),
                                     Data, Scratch,
                                     state, comparator, 0);

    _do_destroy_array(Scratch, deinitElts=false);
  }
}

