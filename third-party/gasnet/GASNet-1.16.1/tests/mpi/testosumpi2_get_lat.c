/*
 * Copyright (C) 2002-2004 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.
 */


#include "mpi.h"
#include "stdio.h"

#define MESSAGE_ALIGNMENT 64
#define MAX_SIZE (1<<22)
#define MYBUFSIZE (MAX_SIZE + MESSAGE_ALIGNMENT)

#define skip 100
#define INER_LOOP 1
#define LOOP 1000

char A[MYBUFSIZE];
char B[MYBUFSIZE];

int main(int argc, char *argv[]) 
{ 
    int rank, destrank, nprocs, i;
    int align_size;

    char * s_buf, *r_buf;
    MPI_Group comm_group, group;
    MPI_Win win;
    int loop;
    int size;
    double t_start, t_end;
        
    MPI_Init(&argc,&argv); 
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 

    if (nprocs != 2) {
        printf("Run this program with 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    align_size = MESSAGE_ALIGNMENT;
    loop = LOOP;                                                                                                                                           
    s_buf =
        (char *) (((unsigned long) A + (align_size - 1)) /
                  align_size * align_size);
    r_buf =
        (char *) (((unsigned long) B + (align_size - 1)) /
                  align_size * align_size);

    bzero(r_buf, MAX_SIZE);
    memset(s_buf, 1, MAX_SIZE);

    if (rank == 0) {
        fprintf(stdout, "# OSU MPI2 One-sided MPI_Get Latency Test (Version 1.0)\n");
        fprintf(stdout, "# Size\t\tLatency (us) \n");
    }
    
    MPI_Comm_group(MPI_COMM_WORLD, &comm_group);
     
    for (size = 0; size <= MAX_SIZE;
         size = (size ? size * 2 : size + 1)) { 

        if (rank == 0) {
            MPI_Win_create(s_buf, size, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win); 
            destrank = 1;
            MPI_Group_incl(comm_group, 1, &destrank, &group);
            MPI_Barrier( MPI_COMM_WORLD);

            for (i=0;i<skip+loop;i++)
            {
                MPI_Win_start(group, 0, win);
                if (i==skip) {
                     t_start=MPI_Wtime();
                }
                MPI_Get(r_buf,size, MPI_CHAR, 1, 0, size, MPI_CHAR, win); 
                MPI_Win_complete(win);  
                MPI_Win_post(group, 0, win);
                MPI_Win_wait(win);
      	    }
            t_end=MPI_Wtime();
        }
        else {  /* rank=1 */
            MPI_Win_create(s_buf, size, 1, MPI_INFO_NULL,MPI_COMM_WORLD, &win);
            destrank = 0;
            MPI_Group_incl(comm_group, 1, &destrank, &group);
    
	        MPI_Barrier( MPI_COMM_WORLD);
    
            for (i=0;i<skip+loop;i++)
            {
                MPI_Win_post(group, 0, win);
                MPI_Win_wait(win);
                MPI_Win_start(group, 0, win);
                MPI_Get(r_buf,size, MPI_CHAR, 0, 0, size, MPI_CHAR, win);
                MPI_Win_complete(win);

            }
        }
        if ( rank == 0 ) {
	        printf("%d\t\t%f\n",size, (t_end-t_start)*1.0e6/loop/2);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Group_free(&group);
   
        MPI_Win_free(&win); 
    }

    MPI_Group_free(&comm_group);
    MPI_Finalize();
    return 0; 
} 
