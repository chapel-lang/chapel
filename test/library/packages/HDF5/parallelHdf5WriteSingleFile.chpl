proc main {
  use BlockDist, HDF5;
  var A = newBlockArr({1..100, 1..100}, c_int);

  for (i,j) in A.domain {
    A[i,j] = (i*1000+j): c_int;
  }

  hdf5WriteDistributedArray("mydata.h5", "dset", A);
}
