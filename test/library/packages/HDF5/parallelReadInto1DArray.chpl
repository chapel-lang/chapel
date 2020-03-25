use HDF5;

config const inputDir = "EXAMPLE_HDF5";
config const dsetName = "/gisette";

proc main {
  var filenameDom = {1..8, 1..1};
  var filenames: [filenameDom] string;
  for (i,j) in filenameDom {
    filenames[i,j] = inputDir + "/datafile_" + i:string + "_" + j:string + ".h5";
  }

  var A = readNamedHDF5FilesInto1DArrayInt(filenames[.., 1], 8, 1, dsetName);

  writeln(A[5999*5000 + 50]);
}
