// ChapelStandard.chpl
//
pragma "no use ChapelStandard"
pragma "export init"
module ChapelStandard {
  use startInitCommDiags;

  // Internal modules.
  use ChapelBase;
  use String;
  use Atomics;
  use NetworkAtomics;
  use NetworkAtomicTypes;
  use AtomicsCommon;
  use ChapelNumLocales;
  use ChapelThreads;
  use ChapelThreadsInternal;
  use ChapelTasksInternal;
  use ChapelTuple;
  use ChapelRange;
  use ChapelReduce;
  use ChapelSyncvar;
  use ChapelLocale;
  use DefaultRectangular; // This might be able to go just after Atomics
  use LocaleModel;
  use LocalesArray;
  use ChapelArray;
  use ChapelDistribution;
  use ChapelIO;
  use LocaleTree;
  use DefaultAssociative;
  use DefaultSparse;
  use DefaultOpaque;
  use ChapelTaskTable;
  use MemTracking;
  use ChapelUtil;
  use ChapelDynDispHack;

  // Standard modules.
  use Types;
  use Math;

  use stopInitCommDiags;
}
