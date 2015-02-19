// Wrappers for the MPI-distributed FFTW
// Maybe this should be a submodule that can be included 
// with a compile-time switch.
module FFTW_MPI {

  use SysCTypes;
  use FFTW;
  use SimpleMPI;

  // Start with a straight wrap of the key FFTW routines
  extern proc fftw_mpi_init();
  extern proc fftw_mpi_cleanup();

  // Data distribution functions
  // TODO : I use ssize_t for everything below, which hopefully matches
  // ptrdiff_t
  extern proc fftw_mpi_local_size(rank : c_int, const ref n : ssize_t, comm : MPI_Comm, 
      ref local_n0 : ssize_t, ref local_0_start : ssize_t) : ssize_t;
  extern proc fftw_mpi_local_size_transposed(rank : c_int, const ref n : ssize_t, 
      comm : MPI_Comm, 
      ref local_n0 : ssize_t, ref local_0_start : ssize_t,
      ref local_n1 : ssize_t, ref local_1_start : ssize_t) : ssize_t;
  // Note 1D transforms are special here
  // Make sure you read the revelant sections in the FFTW manual
  extern proc 



}
