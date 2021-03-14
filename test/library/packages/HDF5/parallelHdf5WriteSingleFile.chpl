config const fileName = "mydata.h5",
             dsetName = "dset";

proc main {
  use BlockDist, HDF5, HDF5.IOusingMPI, SysCTypes;
  use Hdf5PathHelp;

  const pathPrefix = readPrefixEnv();
  var A = newBlockArr({1..100, 1..100}, c_int);

  for (i,j) in A.domain {
    A[i,j] = (i*1000+j): c_int;
  }

  writeln("Writing array");
  hdf5WriteDistributedArray(A, pathPrefix + fileName, dsetName);
  writeln("Wrote array");

  var B = newBlockArr({1..100, 1..100}, c_int);
  writeln("Reading array");
  hdf5ReadDistributedArray(B, pathPrefix + fileName, dsetName);
  writeln("Read array");
  for (i,j) in B.domain {
    if B[i,j] != (i*1000+j): c_int {
      halt("error at index (", i, ", ", j, "). expected: ", i*1000+j, " got: ", B[i,j]);
    }
  }
}
