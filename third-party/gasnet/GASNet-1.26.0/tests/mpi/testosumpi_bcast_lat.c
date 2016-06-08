/*
 * Copyright (C) 2002-2004 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.  
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_MSG_SIZE 200000
#define SKIP 100
#define ITERATIONS 1000
#define ROOT 0
#define LEAF (numprocs - 1)
#define DELAY 100

static /*inline*/ double usecs_delay(int usecs);
double ret_us(void);
void get_ack_time(int);

char x[MAX_MSG_SIZE];
char y[4] = {0,0,0,0};
double ack_time = 0.0;
int numprocs;

int main(int argc, char *argv[])
{
	int i = 0, rank = 0;
        double tmp1 = 0.0, tmp2 = 0.0;
	int size;
	int mpi_errno = 0;
	double latency = 0.0, total = 0.0;
	MPI_Status status;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    if (rank == 0) {
        fprintf(stdout, "# OSU MPI_Bcast Latency Test (Version 1.0)\n");
        fprintf(stdout, "# Size\t\tLatency (us) \n");
    }
    
	MPI_Barrier( MPI_COMM_WORLD );
        get_ack_time(rank); 
    
	for( i=0; i < MAX_MSG_SIZE; i++ ) {
			x[i]='a';
	}
    

	MPI_Barrier( MPI_COMM_WORLD );

    for (size=1; size <= 16384; size *= 2){
	    for ( i=0; i < ITERATIONS + SKIP ; i++ ) {

            if (i == SKIP){
		    if (rank == ROOT) tmp1 = ret_us();
            }

			MPI_Bcast(&x, size, MPI_CHAR, 0, MPI_COMM_WORLD);		

			if ( rank == ROOT ) {
				mpi_errno = MPI_Recv(&y, 0, MPI_CHAR, LEAF, 1, MPI_COMM_WORLD, &status);
                                if (mpi_errno != MPI_SUCCESS) printf("Receive failed\n");
				
			}

			if (rank == LEAF) {
                                usecs_delay(DELAY);
				mpi_errno = MPI_Send(&y, 0, MPI_CHAR, ROOT, 1, MPI_COMM_WORLD);
				if (mpi_errno != MPI_SUCCESS) printf("Send failed\n");
			}
	}

	if (rank == ROOT){
                tmp2 = ret_us();
		total = tmp2 - tmp1;
	}
	
	if ( rank == ROOT )	{
		latency = ( total/ (double)ITERATIONS );
		printf ("%5d	%8.3f\n", 
				 size,latency - DELAY - ack_time );
	}

        MPI_Barrier(MPI_COMM_WORLD);
   }

	MPI_Finalize();
	return 0;
}

void get_ack_time(int myid){

    int i;
    double t_start = 0.0, t_end = 0.0;
    MPI_Status reqstat;

    if (myid == 0) {
        for (i = 0; i <  ITERATIONS + SKIP ; i++) {
            if (i == SKIP)
                t_start = ret_us();
            MPI_Send(x, 0, MPI_CHAR, numprocs-1, 1, MPI_COMM_WORLD);
            MPI_Recv(y, 0, MPI_CHAR, numprocs-1, 1, MPI_COMM_WORLD,
                    &reqstat);
        }
        t_end = ret_us();

    } else if (myid == numprocs-1) {
        for (i = 0; i < ITERATIONS + SKIP; i++) {
            MPI_Recv(y, 0, MPI_CHAR, 0, 1, MPI_COMM_WORLD,
                    &reqstat);
            MPI_Send(x, 0, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        }
    }

    if (myid == 0) {
        ack_time = (t_end - t_start) / (2.0 * ITERATIONS);
    }

}

double usecs_delay(int usecs)
{
    double elapsed, tmp1, tmp2;
    
    tmp1 = ret_us();

    do{
        tmp2 = ret_us();
        elapsed = tmp2 - tmp1;
    } while (elapsed < (double)usecs);        

    return (elapsed);
}    


double ret_us(void)
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double) t.tv_sec * (double) 1e6 + (double) t.tv_usec;
}


