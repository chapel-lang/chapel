use BlockDist, HDF5, FileSystem;

record MyRec {
  var D: domain(1);
  var Ar: [D] real;
  var Ai: [D] int;
  proc init(n: int, low: int) {
    D = {1..n};
    this.complete();
    for i in 1..n {
      Ar[i] = low + i/10.0;
      Ai[i] = low + i/2;
    }
  }
  proc init() {
    init(0, 0);
  }
}

config const nFiles = 5;
config const cleanupFiles = true;

// This directory will be created and removed. Since it will be removed
// don't allow setting it via 'config'.
const hdf5Dir = "hdf5_dir";

proc main {
  var Space = {1..nFiles};
  var BlockSpace = Space dmapped Block(Space, Locales, dataParTasksPerLocale=1);
  var data: [BlockSpace] MyRec;

  // create the directory hdf5Dir
  if !exists(hdf5Dir) then
    mkdir(hdf5Dir);

  // create data
  forall (d, size, low) in zip (data, 100.., 1..) {
    d = new MyRec(size, low);
  }

  var filenames: [1..nFiles] string;
  for i in filenames.domain do filenames[i] = "datafile" + i:string + ".h5";

  // write data to files
  {
    var dsetNames: [1..nFiles] string = "/Ar";
    var Ars: [BlockSpace] ArrayWrapper(data[1].Ar.eltType, data[1].Ar.rank);
    forall (ar, d) in zip(Ars, data) {
      ar = new ArrayWrapper(d.Ar.eltType, d.Ar.rank, d.Ar.domain, d.Ar);
    }
    writeArraysToHDF5Files(hdf5Dir, dsetNames, filenames, data[1].Ar.eltType,
                           data[1].Ar.rank, Ars, Hdf5OpenMode.Truncate);
  }

  {
    var dsetNames: [1..nFiles] string = "/Ai";
    var Ais: [BlockSpace] ArrayWrapper(data[1].Ai.eltType, data[1].Ai.rank);
    forall (ai, d) in zip(Ais, data) {
      ai = new ArrayWrapper(d.Ai.eltType, d.Ai.rank, d.Ai.domain, d.Ai);
    }
    writeArraysToHDF5Files(hdf5Dir, dsetNames, filenames, data[1].Ai.eltType,
                           data[1].Ai.rank, Ais, Hdf5OpenMode.Append);
  }

  // cleanup: remove hdf5Dir and its contents
  if cleanupFiles then
    rmTree(hdf5Dir);
}
