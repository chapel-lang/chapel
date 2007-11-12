#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


#define REPS 1000
int MYTHREAD;
int THREADS;

void print_usage(char *prog) {
  if(MYTHREAD==0) {
    fprintf(stderr, "usage: %s (iters)\n", prog); 
  }
  MPI_Barrier(MPI_COMM_WORLD);
}


void run_bcast_test(int elem_per_thread, int iters, int use_barrier) {
  int *src;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  src = (int*) malloc(sizeof(int)*elem_per_thread);
  if(MYTHREAD==0) {
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
    
    
    for(i=0; i<elem_per_thread; i++) {
       src[i] = (i+1);
     }
  } 
  for(i=0; i<10; i++) {
    MPI_Bcast(src, elem_per_thread, MPI_INT, 0, MPI_COMM_WORLD);

    /*start_time = MPI_Wtime();    
      MPI_Barrier(MPI_COMM_WORLD);
      barrier_time += MPI_Wtime() - start_time; */
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
   
   coll_start_time = MPI_Wtime();
   for(i=0; i<iters; i++) {
     MPI_Bcast(src, elem_per_thread, MPI_INT, 0, MPI_COMM_WORLD);
     if(use_barrier) {
       start_time = MPI_Wtime();
       MPI_Barrier(MPI_COMM_WORLD);
       barrier_time += MPI_Wtime() - start_time;
     }
   }

   if(!use_barrier) { 
     start_time = MPI_Wtime();
     MPI_Barrier(MPI_COMM_WORLD);
     barrier_time += MPI_Wtime() - start_time;
   }
   
   coll_time = MPI_Wtime() - coll_start_time;
   
   if(MYTHREAD!=0) {

     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   } else {
     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, barrier_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, coll_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for(i=0; i<THREADS; i++) {
       if(use_barrier) {
	 fprintf(stdout, "%d> bcast-latency mpi %d (%d bytes) coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
		 elem_per_thread,  elem_per_thread*sizeof(int), 
		 coll_times[i]*1e6 / iters, barrier_times[i]*1e6/iters);
       } else {
	 fprintf(stdout, "%d> bcast-throughput mpi %d (%d bytes) coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
		 elem_per_thread,  elem_per_thread*sizeof(int), 
		 coll_times[i]*1e6 / iters, barrier_times[i]*1e6/iters);
       }

	 
     }
   }
   free(src);
}

void run_scatter_test(int elem_per_thread) {
  double *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  
  dest = (double*) malloc(sizeof(double)*elem_per_thread);
  if(MYTHREAD==0) {
    src = (double*) malloc(sizeof(double)*elem_per_thread*THREADS);
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
    
    
    for(i=0; i<elem_per_thread*THREADS; i++) {
       src[i] = 3.14*(i+1);
     }
   } 
   
   MPI_Barrier(MPI_COMM_WORLD);
   
   coll_start_time = MPI_Wtime();
   for(i=0; i<REPS; i++) {
     MPI_Scatter(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, 
		 0, MPI_COMM_WORLD);
     start_time = MPI_Wtime();
     
     MPI_Barrier(MPI_COMM_WORLD);
     barrier_time += MPI_Wtime() - start_time;
   }
   coll_time = MPI_Wtime() - coll_start_time;
   
   if(MYTHREAD!=0) {

     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   } else {
     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, barrier_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, coll_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for(i=0; i<THREADS; i++) {
       fprintf(stdout, "%d> scatter mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
	       elem_per_thread, 
	       coll_times[i]*1e6 / REPS, barrier_times[i]*1e6/REPS);
     }
   }
   if(MYTHREAD==0) free(src);
   free(dest);
}

void run_gather_test(int elem_per_thread) {
  double *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  src = (double*) malloc(sizeof(double)*elem_per_thread);

  if(MYTHREAD==0) {
    dest = (double*) malloc(sizeof(double)*elem_per_thread*THREADS);
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
    
    
    
  } 
  for(i=0; i<elem_per_thread; i++) {
    src[i] = 3.14*(i+1);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  coll_start_time = MPI_Wtime();
  for(i=0; i<REPS; i++) {
    MPI_Gather(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, 
		 0, MPI_COMM_WORLD);
     start_time = MPI_Wtime();
     
     MPI_Barrier(MPI_COMM_WORLD);
     barrier_time += MPI_Wtime() - start_time;
   }
   coll_time = MPI_Wtime() - coll_start_time;
   
   if(MYTHREAD!=0) {

     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   } else {
     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, barrier_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, coll_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for(i=0; i<THREADS; i++) {
       fprintf(stdout, "%d> gather mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
	       elem_per_thread, 
	       coll_times[i]*1e6 / REPS, barrier_times[i]*1e6/REPS);
     }
   }
   if(MYTHREAD==0) free(dest);
   free(src);
}

void run_reduce_test(int elem_per_thread) {
  double *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  src = (double*) malloc(sizeof(double)*elem_per_thread);

  if(MYTHREAD==0) {
    dest = (double*) malloc(sizeof(double)*elem_per_thread);
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
  } else {
    dest = NULL;
    coll_times = NULL;
    barrier_times = NULL;
  }
  for(i=0; i<elem_per_thread; i++) {
    src[i] = 3.14*(i+1)+((MYTHREAD+1)*100);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  coll_start_time = MPI_Wtime();
  for(i=0; i<REPS; i++) {
    MPI_Reduce(src, dest, elem_per_thread, MPI_DOUBLE, MPI_SUM,  
	       0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    barrier_time += MPI_Wtime() - start_time;
  }
  coll_time = MPI_Wtime() - coll_start_time;
  
  
  if(MYTHREAD!=0) {
    
    MPI_Gather(&barrier_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&coll_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    MPI_Gather(&barrier_time, 1, MPI_DOUBLE, barrier_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&coll_time, 1, MPI_DOUBLE, coll_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for(i=0; i<THREADS; i++) {
      fprintf(stdout, "%d> reduce mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
	      elem_per_thread, 
	      coll_times[i]*1e6 / REPS, barrier_times[i]*1e6/REPS);
    }
  }
  if(MYTHREAD==0) free(dest);
  free(src);
}

void run_all_to_all_test(int elem_per_thread, int iters) {
  int *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  
  dest = (int*) malloc(sizeof(int)*elem_per_thread*THREADS);
  src = (int*) malloc(sizeof(int)*elem_per_thread*THREADS);
  if(MYTHREAD==0) {

    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
    
    

   } 
  for(i=0; i<elem_per_thread*THREADS; i++) {
    src[i] = (MYTHREAD+1);
  }
   MPI_Barrier(MPI_COMM_WORLD);
   
   for(i=0; i<10; i++) {
     MPI_Alltoall(src, elem_per_thread, MPI_INT, dest, elem_per_thread, MPI_INT, 
		 MPI_COMM_WORLD);
     MPI_Barrier(MPI_COMM_WORLD);
   }
   
   MPI_Barrier(MPI_COMM_WORLD); 
   
   coll_start_time = MPI_Wtime();
   for(i=0; i<iters; i++) {
     MPI_Alltoall(src, elem_per_thread, MPI_INT, dest, elem_per_thread, MPI_INT, 
		 MPI_COMM_WORLD);

   }
   start_time = MPI_Wtime();
   
   MPI_Barrier(MPI_COMM_WORLD);
   barrier_time += MPI_Wtime() - start_time;
   coll_time = MPI_Wtime() - coll_start_time;
   
   if(MYTHREAD!=0) {

     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   } else {
     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, barrier_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, coll_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for(i=0; i<THREADS; i++) {
       fprintf(stdout, "%d> alltoall mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
	       elem_per_thread, coll_times[i]*1e6 / iters, barrier_times[i]*1e6);
     }
   }
   free(src);
   free(dest);
}

void run_all_reduce_all_test(int elem_per_thread, int iters) {
  double *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  
  dest = (double*) malloc(sizeof(double)*elem_per_thread);
  src = (double*) malloc(sizeof(double)*elem_per_thread);
  if(MYTHREAD==0) {
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
  } 
  for(i=0; i<elem_per_thread; i++) {
    src[i] = 3.14*(MYTHREAD+1);
  }
   MPI_Barrier(MPI_COMM_WORLD);
   
   coll_start_time = MPI_Wtime();
   for(i=0; i<REPS; i++) {
     MPI_Allreduce(src, dest, elem_per_thread, MPI_DOUBLE, MPI_SUM, 
		 MPI_COMM_WORLD);
   }
   coll_time = MPI_Wtime() - coll_start_time;
   
   if(MYTHREAD!=0) {

     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   } else {
     MPI_Gather(&barrier_time, 1, MPI_DOUBLE, barrier_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     MPI_Gather(&coll_time, 1, MPI_DOUBLE, coll_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
     for(i=0; i<THREADS; i++) {
       fprintf(stdout, "%d> allreduce mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
	       elem_per_thread, coll_times[i]*1e6 / REPS, barrier_times[i]*1e6/REPS);
     }
   }
   free(src);
   free(dest);
}

int main(int argc, char **argv) {

  int elem_per_thread;
  double start_time;
  double stop_time;
  int i;
  int iters;
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &MYTHREAD);
  MPI_Comm_size(MPI_COMM_WORLD, &THREADS);

  
  switch(argc) {
  case 1: iters=10000; break;
  case 2: iters=atoi(argv[1]); break;
  case 3: print_usage(argv[0]); MPI_Finalize(); return 1; break;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  for(i=0; i<REPS*10; i++) {
    MPI_Barrier(MPI_COMM_WORLD);
  }
  stop_time = MPI_Wtime();
  if(MYTHREAD==0) {
    fprintf(stdout, "%d> barrier mpi %.3f\n", MYTHREAD, ((stop_time - start_time)*1e6)/(REPS*10));
  }  
  
  for(elem_per_thread = 1; elem_per_thread<=2048; elem_per_thread*=2) {
    run_bcast_test(elem_per_thread, iters, 1);
    run_bcast_test(elem_per_thread, iters, 0);
    run_all_to_all_test(elem_per_thread, iters);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
