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
  extern proc fftw_mpi_local_size(rank : c_int, const ref n : c_ptrdiff, comm : MPI_Comm, 
      ref local_n0 : c_ptrdiff, ref local_0_start : c_ptrdiff) : c_ptrdiff;
  extern proc fftw_mpi_local_size_transposed(rank : c_int, const ref n : c_ptrdiff, 
      comm : MPI_Comm, 
      ref local_n0 : c_ptrdiff, ref local_0_start : c_ptrdiff,
      ref local_n1 : c_ptrdiff, ref local_1_start : c_ptrdiff) : c_ptrdiff;
  // Note 1D transforms are special here
  // Make sure you read the revelant sections in the FFTW manual
  extern proc fftw_mpi_local_size_1d(n0 : c_ptrdiff, comm : MPI_Comm, 
      sign : c_int, flags : c_uint, 
      ref local_ni : c_ptrdiff, ref local_i_start : c_ptrdiff,
      ref local_no : c_ptrdiff, ref local_o_start : c_ptrdiff) : c_ptrdiff;


  // Planner routines
  extern proc fftw_mpi_plan_dft(rank: c_int, 
      n,  // BLC: having trouble being specific
      in1: [] fftw_complex, 
      out1: [] fftw_complex, 
      comm : MPI_Comm, 
      sign : c_int, flags : c_uint) : fftw_plan;
  extern proc fftw_plan_dft_r2c(rank: c_int, 
      n,  // BLC: having trouble being specific
      in1: [],
      out1: [], 
      comm : MPI_Comm, 
      flags : c_uint) : fftw_plan;
  extern proc fftw_plan_dft_c2r(rank: c_int, 
      n,  // BLC: having trouble being specific
      in1: [],
      out1: [],
      comm : MPI_Comm,
      flags : c_uint) : fftw_plan;
}
