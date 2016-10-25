use MPI;

var rank = commRank(CHPL_COMM_WORLD),
    size = commSize(CHPL_COMM_WORLD);

for irank in 0.. #size {
  if irank == rank then
    writef("Hello, Chapel! This is MPI rank=%i of size=%i, on locale.id=%i\n",rank, size, here.id);
  C_MPI.MPI_Barrier(CHPL_COMM_WORLD);
}
