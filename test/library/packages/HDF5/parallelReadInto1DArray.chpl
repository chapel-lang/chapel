use HDF5;
use Hdf5PathHelp;

config const inputDir = "EXAMPLE_HDF5";
config const dsetName = "/gisette";



proc main {
  var filenameDom = {1..8, 1..1};
  var filenames: [filenameDom] string;
  const prefixPath = readPrefixEnv();
  if prefixPath != "" {
    use FileSystem;
    copyTree(inputDir, prefixPath + inputDir);
  }

  for (i,j) in filenameDom {
    filenames[i,j] = prefixPath + inputDir + "/datafile_" + i:string + "_" + j:string + ".h5";
  }

  var A = readNamedHDF5FilesInto1DArrayInt(filenames[.., 1], 8, 1, dsetName);

  writeln(A[5999*5000 + 50]);

  if prefixPath != "" {
    use FileSystem;
    rmTree(prefixPath + inputDir);
  }

}
