/*
 * Copyright (C) 2002-2004 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.
 */

#include "mpi.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MYBUFSIZE (150000000)  /*~= 100M Bytes -RN, correction*/
#define MAX_REQ_NUM 100

/* Note we have a upper limit for buffer size, so be extremely careful
 * if you want to change the loop size or warm up size */
#define WARMUP	(10)
#define MAX_SIZE (1<<22)
#define LOOP (30)

#define WINDOW_SIZE	(32)

char s_buf1[MAX_SIZE+128];
char r_buf1[MYBUFSIZE];

MPI_Request request[MAX_REQ_NUM];

int main(int argc,char *argv[])
{
    int  myid, numprocs, i, j;
    int size, loop, page_size;
    char *s_buf, *r_buf;
    double t_start=0.0, t_end=0.0, t=0.0;
    int destrank;
    MPI_Group comm_group, group;
    MPI_Win win;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_group(MPI_COMM_WORLD, &comm_group);

    loop = LOOP;
    page_size = getpagesize();

    s_buf = (char*)(((unsigned long)s_buf1 + (page_size -1))/page_size * page_size);
    r_buf = (char*)(((unsigned long)r_buf1 + (page_size -1))/page_size * page_size);

    assert( (s_buf != NULL) && (r_buf != NULL) );
    assert( MAX_SIZE * WINDOW_SIZE < MYBUFSIZE );

    if (myid == 0) {
        fprintf(stdout, "# OSU MPI2 MPI_Get Bandwidth Test (Version 1.0)\n");
        fprintf(stdout, "# Modified to report: 1 MB == 2^20 bytes\n");
        fprintf(stdout, "# Size\t\tBandwidth (MB/s) \n");
    }

	/* Bandwidth test */
    for(size=1 ;size<=MAX_SIZE; size*=2)
	{
	    /* Window creation and warming-up */
        if (myid == 0)
        {
            MPI_Win_create(s_buf, size , 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
            
            destrank = 1;
            MPI_Group_incl(comm_group, 1, &destrank, &group);
            	    
            for ( i=0; i< WARMUP; i++ ) {
                MPI_Win_start(group, 0, win);
                MPI_Get((char *)((unsigned long)r_buf+i*size), size, MPI_CHAR, 1, 0, size, MPI_CHAR, win);
                MPI_Win_complete(win);	
            } 
        }else{
            MPI_Win_create(s_buf, size, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win );
            destrank = 0;
            MPI_Group_incl(comm_group, 1, &destrank, &group);

            for ( i=0; i< WARMUP; i++ ) {
                MPI_Win_post(group, 0, win);
                MPI_Win_wait(win);
            }	
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if (myid == 0)
        {
            t_start=MPI_Wtime();
            for ( i=0; i< loop; i++ )
            {
                MPI_Win_start(group, 0, win);
                for(j=0;j<WINDOW_SIZE;j++)
                {
                    MPI_Get((char *)((unsigned long)r_buf+j*size), size, MPI_CHAR, 1, 0,size, MPI_CHAR, win);
                }
                MPI_Win_complete(win); 
            }
            t_end=MPI_Wtime();
            MPI_Barrier(MPI_COMM_WORLD);
            t = t_end - t_start;
        }
        else{
            for ( i=0; i< loop; i++ )
            {
                MPI_Win_post(group, 0, win);
                MPI_Win_wait(win);	
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
        if ( myid == 0 ) {
            double tmp;
         #if 0
            tmp = ((size*1.0)/(1000000))*loop*WINDOW_SIZE; 
         #else
            /* DOB: this is the only change from the official MVAPICH tester
             * report MB as 1MB = 2^20 bytes
            */
            tmp = ((size*1.0)/(1024*1024))*loop*WINDOW_SIZE; 
         #endif
            fprintf(stdout,"%d\t\t%f\n", size, tmp/t);
        }
        MPI_Win_free(&win);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
