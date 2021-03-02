
int MPI_Partitioned_Send_create(const void *buf, int count, MPI_Datatype datatype, int numThreads,
                                         int dest, int tag, MPI_Comm comm,
                                                                          MPI_Request *request);
int MPI_Partitioned_Recv_create(void *buf, int count, MPI_Datatype datatype, int source,
                                         int tag, MPI_Comm comm, MPI_Request *request);

int MPI_Partitioned_Add_to_buffer( MPI_Request*, const void* send_buf, int count, MPI_Datatype );

int MPI_Partitioned_free( MPI_Request* );

int MPI_Wait_part(MPI_Request *request, MPI_Status *status);

int MPI_Start_part(MPI_Request *request);
