use HDF5;

config const infileName = "readChunks2DInput.h5";
config const dsetName = "/dset";

for A in hdf5ReadChunks(infileName, dsetName,
                        chunkShape={1..5, 1..5}, eltType=int) {
  writeln(A);
  writeln();
}
