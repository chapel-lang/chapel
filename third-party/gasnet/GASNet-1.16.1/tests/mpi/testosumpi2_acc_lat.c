
/*
 * Copyright (C) 2002-2004 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.
 */

#include "mpi.h"
#include <stdio.h>

#define SKIP 100
#define LOOP 1000

#define MESSAGE_ALIGNMENT 64
#define MAX_SIZE (1<<22)
#define MYBUFSIZE (MAX_SIZE + MESSAGE_ALIGNMENT)

char s_buf_original[MYBUFSIZE]; 
char r_buf_original[MYBUFSIZE];

int main( int argc, char *argv[] )
{

    int rank, destrank, nprocs, i;
    MPI_Group comm_group, group;
    MPI_Win win;

    int loop;
    int size;
    double t_start, t_end;

    int count, align_size;
    int * s_buf;
    int * r_buf;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_group(MPI_COMM_WORLD, &comm_group);

    loop        = LOOP;
    align_size  = MESSAGE_ALIGNMENT;

    s_buf =
        (int *) (((unsigned long) s_buf_original + (align_size - 1)) /
                  align_size * align_size);
    r_buf =
        (int *) (((unsigned long) r_buf_original + (align_size - 1)) /
                  align_size * align_size);

    for (i=0; i < MAX_SIZE/sizeof(int); i++)
    {
      r_buf[i] = i;
      s_buf[i] = 2*i;

    }

    if (rank == 0) {
        fprintf(stdout, "# OSU MPI2 Accumulate Latency Test (Version 1.0)\n");
        fprintf(stdout, "# Size\t\tLatency (us) \n");
    }

    for (count = 0; count <= MAX_SIZE/sizeof(int);
         count = (count ? count * 2 : count + 1)) {
      size = count * sizeof(int);
      if (rank == 0)
      {
            MPI_Win_create(r_buf, size, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
            destrank = 1;
            MPI_Group_incl(comm_group, 1, &destrank, &group);
            MPI_Barrier( MPI_COMM_WORLD);
      
            for (i=0;i<SKIP+loop;i++)
            {
                MPI_Win_start(group, 0, win);
                if (i==SKIP) t_start=MPI_Wtime();
                MPI_Accumulate(s_buf, count, MPI_INT, 1, 0, count, MPI_INT,MPI_SUM, win);
                MPI_Win_complete(win);
                MPI_Win_post(group, 0, win);
                MPI_Win_wait(win);
            }
            t_end=MPI_Wtime();
            MPI_Win_free(&win);

      }
      else
      {
            MPI_Win_create(r_buf, size, 1, MPI_INFO_NULL,MPI_COMM_WORLD, &win);
            destrank = 0;
            MPI_Group_incl(comm_group, 1, &destrank, &group);
            MPI_Barrier( MPI_COMM_WORLD);

            for (i=0;i<SKIP+loop;i++)
            {
                MPI_Win_post(group, 0, win);
                MPI_Win_wait(win);
                MPI_Win_start(group, 0, win);
                MPI_Accumulate(s_buf, count, MPI_INT, 0, 0, count, MPI_INT,MPI_SUM, win);
                MPI_Win_complete(win);

            }
            MPI_Win_free(&win);

      }

      if(rank == 0)
      {
        printf("%d\t\t%f\n",size, (t_end-t_start)*1.0e6/loop/2);
        fflush(stdout);  
      }

   } 

  MPI_Finalize();
  return 0; 
}
