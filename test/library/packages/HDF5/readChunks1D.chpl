use HDF5_HL;

config const infileName = "readChunks1DInput.h5";
config const dsetName = "Ai";

for A in hdf5ReadChunks(infileName, dsetName,
                        outRank=1, chunkShape={1..8}, eltType=int) {
  writeln(A);
}
