use HDF5, Hdf5PathHelp;

config const infileName = "readChunks2DInput.h5";
config const dsetName = "/dset";

const pathPrefix = readPrefixEnv();
if pathPrefix != "" {
  use FileSystem;
  copy(infileName, pathPrefix + infileName);
}

for A in hdf5ReadChunks(pathPrefix+infileName, dsetName,
                        chunkShape={1..5, 1..5}, eltType=int) {
  writeln(A);
  writeln();
}
