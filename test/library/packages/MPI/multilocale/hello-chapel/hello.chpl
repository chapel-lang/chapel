use MPI;

coforall loc in Locales do on loc {
  var rank = commRank(CHPL_COMM_WORLD),
      size = commSize(CHPL_COMM_WORLD);

  for irank in 0.. #size {
    if irank == rank then
      writef("Hello, Chapel! This is MPI rank=%i of size=%i, on locale.id=%i\n",rank, size, here.id);

    /* Why the double barrier?

       A single barrier is to ensure that the code outputs in the correct sequence, and
       doesn't overwrite. This could be achieved with a single Barrier.

       Having a direct call into the MPI_Barrier also tests that the first Barrier prevents
       deadlocks when running with CHPL_TASKS=qthreads. This second MPI_Barrier call stands 
       in for any blocking MPI call.
    */
    Barrier(CHPL_COMM_WORLD);
    C_MPI.MPI_Barrier(CHPL_COMM_WORLD);
  }
}
