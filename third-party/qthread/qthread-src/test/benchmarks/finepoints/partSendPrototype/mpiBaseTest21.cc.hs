#include <omp.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpiPart.h>

double doTest21( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
{
	MPI_Request sendReq,recvReq;
	int other = (rank + 1) % 2;
	double start;
	int TAG = 0xdead;
	int rc;

    if ( rank == 0 ) {
		rc = MPI_Partitioned_Send_create( sendBuf, threadPart * numThreads, MPI_CHAR, numThreads, other, TAG,
                    MPI_COMM_WORLD, &sendReq );
		assert( rc == MPI_SUCCESS );

	} else {
		rc = MPI_Partitioned_Recv_create( recvBuf, threadPart * numThreads, MPI_CHAR, other, TAG,
                   	MPI_COMM_WORLD, &recvReq );
		assert( rc == MPI_SUCCESS );
    }

	start = MPI_Wtime();

#pragma omp parallel shared(rank,numIterations,sendBuf,recvBuf,numThreads,threadPart,sendReq,recvReq) num_threads(numThreads)
	{
	    int rc;
	    int tid = omp_get_thread_num();
	    int iteration;

		for ( iteration = 0; iteration < numIterations; iteration++ ) {

#pragma omp master 
			{
                if ( 0 == rank ) {
#if 0
                    rc = MPI_Recv( NULL, 0, MPI_CHAR, other, 0xf00d, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
                    assert( rc == MPI_SUCCESS );
#endif
					rc = MPI_Start(&sendReq);
					assert( rc == MPI_SUCCESS );
                } else {
#if 0
                    rc = MPI_Send( NULL, 0, MPI_CHAR, other, 0xf00d, MPI_COMM_WORLD );
                    assert( rc == MPI_SUCCESS );
#endif
                }
			}
          	if ( rank == 0 ) {
#pragma omp barrier
				rc = MPI_Partitioned_Add_to_buffer( &sendReq, sendBuf + (threadPart * tid), threadPart, MPI_CHAR );
            	assert( rc  == MPI_SUCCESS );
			}

#pragma omp master 
			{
            	if ( 1 == rank ) {
					rc = MPI_Wait(&recvReq, MPI_STATUS_IGNORE );
					assert( rc == MPI_SUCCESS );
				}
			}
		}
	}
	if ( 0 == rank ) {
		MPI_Partitioned_free( &sendReq );
	} else {
		MPI_Partitioned_free( &recvReq );
	}

	return MPI_Wtime() - start;
}
