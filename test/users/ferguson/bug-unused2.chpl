module OuterModule {
  module TwoArrayPartitioning {
    param maxBuckets = 512;

    record TwoArrayBucketizerPerTaskState {
      var localCounts = maxBuckets;
    }

  }

  writeln("Hello");
}
