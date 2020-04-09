use HDF5, HDF5Preprocessors;

config const infileName = "readChunks2DInput.h5";
config const dsetName = "/dset";

var addOne = new owned AddNPreprocessor(1);

for A in hdf5ReadChunks(infileName, dsetName,
                        chunkShape={1..5, 1..5}, eltType=int, addOne) {
  writeln(A);
  writeln();
}
