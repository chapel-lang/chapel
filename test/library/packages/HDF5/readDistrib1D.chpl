config const fileName = "dataset1.h5",
             dsetName = "1Di64";

config param testBlock       = true,
             testCyclic      = true,
             testBlockCyclic = false;

proc main {
  use HDF5, HDF5.IOusingMPI;

  var Space = {1..100};

  if testBlock {
    use BlockDist;
    writeln("BlockDist:");

    var BlockSpace = Space dmapped Block(boundingBox=Space);
    var A: [BlockSpace] int;

    hdf5ReadDistributedArray(A, fileName, dsetName);
    writeln(A);
  }

  if testCyclic {
    use CyclicDist;
    writeln("CyclicDist:");
    var CyclicSpace = Space dmapped Cyclic(startIdx=Space.low);
    var A: [CyclicSpace] int;
    hdf5ReadDistributedArray(A, fileName, dsetName);
    writeln(A);
  }

  if testBlockCyclic {
    use BlockCycDist;
    writeln("BlockCyclicDist:");
    var BlockCyclicSpace = Space dmapped BlockCyclic(startIdx=Space.low,
                                                     blocksize=10);
    var A: [BlockCyclicSpace] int;
    hdf5ReadDistributedArray(A, fileName, dsetName);
    writeln(A);
  }
}
