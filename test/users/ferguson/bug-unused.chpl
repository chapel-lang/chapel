module OuterModule {
  module TwoArrayPartitioning {
    param maxBuckets = 512;

    record TwoArrayBucketizerPerTaskState {
      var localCounts: [0..#maxBuckets] int;
    }

  }

  writeln("Hello");
}
