use MPI;

var rank = commRank(CHPL_COMM_WORLD),
    size = commSize(CHPL_COMM_WORLD);

var value = rank + 1, total: c_int;
assert(C_MPI.MPI_Allreduce(value, total, 1, MPI_INT, MPI_SUM,
                          MPI_COMM_WORLD) == MPI_SUCCESS);
assert(total == size * (size + 1) / 2);

for irank in 0.. #size {
  if irank == rank then
    writef("Hello, Chapel! This is MPI rank=%i of size=%i, on locale.id=%i\n",rank, size, here.id);
  C_MPI.MPI_Barrier(CHPL_COMM_WORLD);
}
