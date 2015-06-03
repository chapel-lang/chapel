/*
 * Copyright (C) 2002-2004 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.  
 */

#include "mpi.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define MAX_REQ_NUM 1000

#define MAX_ALIGNMENT 16384
#define MAX_MSG_SIZE (1<<22)
#define MYBUFSIZE (MAX_MSG_SIZE + MAX_ALIGNMENT)

int loop = 100;
int window_size = 64;
int skip = 10;

int loop_large = 20;
int window_size_large = 64;
int skip_large = 2;

int large_message_size = 8192;

char s_buf1[MYBUFSIZE];
char r_buf1[MYBUFSIZE];

MPI_Request request[MAX_REQ_NUM];
MPI_Status  reqstat[MAX_REQ_NUM];

int main(int argc, char *argv[])
{

    int myid, numprocs, i, j;
    int size, align_size;
    char *s_buf, *r_buf;
    double t_start = 0.0, t_end = 0.0, t = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    align_size = getpagesize();
    s_buf =
        (char *) (((unsigned long) s_buf1 + (align_size - 1)) /
                  align_size * align_size);
    r_buf =
        (char *) (((unsigned long) r_buf1 + (align_size - 1)) /
                  align_size * align_size);

    if (myid == 0) {
        fprintf(stdout, "# OSU MPI Bandwidth Test (Version 2.0)\n");
        fprintf(stdout, "# Modified to report: 1 MB == 2^20 bytes\n");
        fprintf(stdout, "# Size\t\tBandwidth (MB/s)  \n");
    }

    /* Bandwidth test */
    for (size = 1; size <= MAX_MSG_SIZE; size *= 2) {

        /* touch the data */
        for (i = 0; i < size; i++) {
            s_buf[i] = 'a';
            r_buf[i] = 'b';
        }

        if (size > large_message_size) {
            loop = loop_large;
            skip = skip_large;
            window_size = window_size_large;
        }

        if (myid == 0) {

            for (i = 0; i < loop + skip; i++) {

                if (i == skip)
                    t_start = MPI_Wtime();
                for (j = 0; j < window_size; j++)
                    MPI_Isend(s_buf, size, MPI_CHAR, 1, 100,
                              MPI_COMM_WORLD, request + j);

                MPI_Waitall(window_size, request, reqstat);
                MPI_Recv(r_buf, 4, MPI_CHAR, 1, 101, MPI_COMM_WORLD,
                         &reqstat[0]);

            }

            t_end = MPI_Wtime();
            t = t_end - t_start;

        } else if (myid == 1) {

            for (i = 0; i < loop + skip; i++) {

                for (j = 0; j < window_size; j++)
                    MPI_Irecv(r_buf, size, MPI_CHAR, 0, 100,
                              MPI_COMM_WORLD, request + j);

                MPI_Waitall(window_size, request, reqstat);
                MPI_Send(s_buf, 4, MPI_CHAR, 0, 101, MPI_COMM_WORLD);

            }

        }

        if (myid == 0) {
            double tmp;
         #if 0
            tmp = ((size * 1.0) / (1000 * 1000)) * loop * window_size;
         #else
            /* DOB: this is the only change from the official MVAPICH tester
             * report MB as 1MB = 2^20 bytes, for consistency with the rest
             * of computer science 
            */
            tmp = ((size * 1.0) / (1024 * 1024)) * loop * window_size;
         #endif
            fprintf(stdout, "%d\t\t%f\n", size, tmp / t);
        }

    }

    MPI_Finalize();
    return 0;

}

