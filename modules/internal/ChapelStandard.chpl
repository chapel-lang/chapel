// ChapelStandard.chpl
//
pragma "no use ChapelStandard"
module ChapelStandard {
  // Internal modules.
  use ChapelBase;
  use Atomics;
  use ChapelNumLocales;
  use ChapelThreads;
  use ChapelThreadsInternal;
  use ChapelTasksInternal;
  use ChapelTuple;
  use ChapelRange;
  use ChapelReduce;
  use ChapelArray;
  use ChapelDistribution;
  use ChapelLocale;
  use ChapelIO;
  use DefaultRectangular; // Must precede ChapelTaskTable.
  use DefaultAssociative;
  use DefaultSparse;
  use DefaultOpaque;
  use ChapelTaskTable;
  use ChapelUtil;

  // Standard modules.
  use Types;
  use Math;
}
