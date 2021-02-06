use HDF5, Hdf5PathHelp;

config const infileName = "readChunks1DInput.h5";
config const dsetName = "Ai";

const pathPrefix = readPrefixEnv();
if pathPrefix != "" {
  use FileSystem;
  copyFile(infileName, pathPrefix + infileName);
}

for A in hdf5ReadChunks(pathPrefix + infileName, dsetName,
                        chunkShape={1..8}, eltType=int) {
  writeln(A);
}
