use HDF5;

config const infileName = "readChunks1DInput.h5";
config const dsetName = "Ai";

for A in hdf5ReadChunks(infileName, dsetName,
                        chunkShape={1..8}, eltType=int) {
  writeln(A);
}
