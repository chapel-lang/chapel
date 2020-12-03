use HDF5, HDF5Preprocessors, Hdf5PathHelp;

config const infileName = "readChunks2DInput.h5";
config const dsetName = "/dset";

const pathPrefix = readPrefixEnv();
if pathPrefix != "" {
  use FileSystem;
  copy(infileName, pathPrefix + infileName);
}


var addOne = new owned AddNPreprocessor(1);

for A in hdf5ReadChunks(pathPrefix+infileName, dsetName,
                        chunkShape={1..5, 1..5}, eltType=int, addOne) {
  writeln(A);
  writeln();
}
