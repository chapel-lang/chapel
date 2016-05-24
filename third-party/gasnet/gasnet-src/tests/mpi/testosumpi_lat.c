/*
 * Copyright (C) 2002-2004 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.  
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define MESSAGE_ALIGNMENT 64
#define MAX_MSG_SIZE (1<<22)
#define MYBUFSIZE (MAX_MSG_SIZE + MESSAGE_ALIGNMENT)

int zero_ack = 1;

char s_buf_original[MYBUFSIZE];
char r_buf_original[MYBUFSIZE];

int skip = 1000;
int loop = 10000;
int skip_large = 10;
int loop_large = 100;
int large_message_size = 8192;

int main(int argc, char *argv[])
{

    int myid, numprocs, i;
    int size;
    MPI_Status reqstat;
    char *s_buf, *r_buf;
    int align_size;

    double t_start = 0.0, t_end = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (argc > 1 && (!strcmp(argv[1],"-n") || !strcmp(argv[1],"-N"))) zero_ack = 0;

    align_size = MESSAGE_ALIGNMENT;

    s_buf =
        (char *) (((unsigned long) s_buf_original + (align_size - 1)) /
                  align_size * align_size);
    r_buf =
        (char *) (((unsigned long) r_buf_original + (align_size - 1)) /
                  align_size * align_size);

    if (myid == 0) {
        fprintf(stdout, "# OSU MPI Ping-pong Latency Test (Version 2.0)\n");
        fprintf(stdout, "# Modified to report round-trip latency (MPI_Send/MPI_Recv)\n");
        if (zero_ack) fprintf(stdout, "# Using a ZERO-byte ack (comparable to GASNet testsmall)\n");
        else fprintf(stdout, "# Using an N-byte ack (comparable to GASNet testam)\n");
        fprintf(stdout, "# Size\t\tLatency (us) \n");
    }

    for (size = 0; size <= MAX_MSG_SIZE;
         size = (size ? size * 2 : size + 1)) {

        /* touch the data */
        for (i = 0; i < size; i++) {
            s_buf[i] = 'a';
            r_buf[i] = 'b';
        }

        if (size > large_message_size) {
            loop = loop_large;
            skip = skip_large;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if (myid == 0) {
            for (i = 0; i < loop + skip; i++) {
                if (i == skip)
                    t_start = MPI_Wtime();
                MPI_Send(s_buf, size, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
               if (zero_ack) 
                MPI_Recv(r_buf, 0, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &reqstat);
               else
                MPI_Recv(r_buf, size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &reqstat);
            }
            t_end = MPI_Wtime();

        } else if (myid == 1) {
            for (i = 0; i < loop + skip; i++) {
                MPI_Recv(r_buf, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &reqstat);
               if (zero_ack) 
                MPI_Send(s_buf, 0, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
               else
                MPI_Send(s_buf, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
            }
        }

        if (myid == 0) {
            double latency;
          #if 0
            latency = (t_end - t_start) * 1.0e6 / (2.0 * loop);
          #else
            /* DOB: the only change - report round-trip latency */
            latency = (t_end - t_start) * 1.0e6 / (loop);
          #endif
            fprintf(stdout, "%d\t\t%0.2f\n", size, latency);
        }

    }

    MPI_Finalize();
    return 0;

}

