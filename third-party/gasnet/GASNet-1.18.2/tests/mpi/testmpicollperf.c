#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


#define REPS iters
int iters;
int MYTHREAD;
int THREADS;

void print_usage(char *prog) {
  if(MYTHREAD==0) {
    fprintf(stderr, "usage: %s (iters) (max size)\n", prog); 
  }
  MPI_Barrier(MPI_COMM_WORLD);
}


void run_bcast_test(int elem_per_thread, int use_barrier) {
  double *src;
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  src = (double*) malloc(sizeof(double)*elem_per_thread);
  if(MYTHREAD==0) {
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
    
    
    for(i=0; i<elem_per_thread; i++) {
       src[i] = (i+3.14);
     }
  } 
  for(i=0; i<10; i++) {
    MPI_Bcast(src, elem_per_thread, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /*start_time = MPI_Wtime();    
      MPI_Barrier(MPI_COMM_WORLD);
      barrier_time += MPI_Wtime() - start_time; */
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
   
   coll_start_time = MPI_Wtime();
   for(i=0; i<iters; i++) {
     MPI_Bcast(src, elem_per_thread, MPI_DOUBLE, 0, MPI_COMM_WORLD);
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
       char *use_barrier_str = "-latency";
       char *use_nobarrier_str = "-throughput";
       
       fprintf(stdout, "%d> bcast-%s mpi %d (%d bytes) coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
               (use_barrier ? use_barrier_str : use_nobarrier_str), elem_per_thread,   elem_per_thread*sizeof(double), 
               coll_times[i]*1e6 / iters, barrier_times[i]*1e6/iters);
     }
   }
   free(src);
}

void run_scatter_test(int elem_per_thread, int use_barrier) {
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
  for(i=0; i<10; i++) {
    MPI_Scatter(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, 
                0, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);
   
   coll_start_time = MPI_Wtime();
   for(i=0; i<iters; i++) {
     MPI_Scatter(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, 
		 0, MPI_COMM_WORLD);
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
       char *use_barrier_str = "-latency";
       char *use_nobarrier_str = "-throughput";
       
       fprintf(stdout, "%d> scatter-%s mpi %d (%d bytes) coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, (use_barrier ? use_barrier_str : use_nobarrier_str),
               elem_per_thread, elem_per_thread*sizeof(double), 
               coll_times[i]*1e6 / iters, barrier_times[i]*1e6/iters);
     }
   }
   if(MYTHREAD==0) free(src);
   free(dest);
}

void run_gather_test(int elem_per_thread, int use_barrier) {
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
  
  for(i=0; i<10; i++) {
    MPI_Gather(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, 
               0, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  
  coll_start_time = MPI_Wtime();
  for(i=0; i<iters; i++) {
    MPI_Gather(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, 
		 0, MPI_COMM_WORLD);
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
       char *use_barrier_str = "-latency";
       char *use_nobarrier_str = "-throughput";
     
       fprintf(stdout, "%d> gather-%s mpi %d (%d bytes) coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, (use_barrier ? use_barrier_str : use_nobarrier_str),
               elem_per_thread, elem_per_thread*sizeof(double),
	       coll_times[i]*1e6 / iters, barrier_times[i]*1e6/iters);
     }
   }
   if(MYTHREAD==0) free(dest);
   free(src);
}


void run_all_gather_test(int elem_per_thread) {
  double *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  src = (double*) malloc(sizeof(double)*elem_per_thread);


  dest = (double*) malloc(sizeof(double)*elem_per_thread*THREADS);
  if(MYTHREAD==0) {
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
  } 
  for(i=0; i<elem_per_thread; i++) {
    src[i] = 3.14*(i+1);
  }
  
  for(i=0; i<10; i++) {
    MPI_Allgather(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  
  coll_start_time = MPI_Wtime();
  for(i=0; i<iters; i++) {
    MPI_Allgather(src, elem_per_thread, MPI_DOUBLE, dest, elem_per_thread, MPI_DOUBLE, MPI_COMM_WORLD);
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
       fprintf(stdout, "%d> all_gather mpi %d (%d bytes) coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
               elem_per_thread, elem_per_thread*sizeof(double),
	       coll_times[i]*1e6 / iters, barrier_times[i]*1e6/iters);
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
      fprintf(stdout, "%d> reduce_double mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
	      elem_per_thread, 
	      coll_times[i]*1e6 / REPS, barrier_times[i]*1e6/REPS);
    }
  }
  if(MYTHREAD==0) free(dest);
  free(src);
}

void run_int_reduce_test(int elem_per_thread, int use_barrier) {
  int *src, *dest;
 
  
  double start_time;
  double barrier_time=0;
  double coll_time;
  double coll_start_time;
  double *coll_times;
  double *barrier_times;
  int i;

  src = (int*) malloc(sizeof(int)*elem_per_thread);

  if(MYTHREAD==0) {
    dest = (int*) malloc(sizeof(int)*elem_per_thread);
    coll_times = (double*) malloc(sizeof(double)*THREADS);
    barrier_times = (double*) malloc(sizeof(double)*THREADS);
  } else {
    dest = NULL;
    coll_times = NULL;
    barrier_times = NULL;
  }
  for(i=0; i<elem_per_thread; i++) {
    src[i] = 42+i;
  }
  for(i=0; i<10; i++) {
    MPI_Reduce(src, dest, elem_per_thread, MPI_INT, MPI_SUM,  
	       0, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  
  coll_start_time = MPI_Wtime();
  for(i=0; i<iters; i++) {
    MPI_Reduce(src, dest, elem_per_thread, MPI_INT, MPI_SUM,  
	       0, MPI_COMM_WORLD);
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
      char *use_barrier_str = "-latency";
      char *use_nobarrier_str = "-throughput";
     

      fprintf(stdout, "%d> reduce_int-%s mpi %d coll_time: %.3f +-0%% barrier_time: %.3f +-0%%\n",i, 
              (use_barrier ? use_barrier_str : use_nobarrier_str),
                 elem_per_thread, 
                 coll_times[i]*1e6 / REPS, barrier_times[i]*1e6/REPS);
    }
  }
  if(MYTHREAD==0) free(dest);
  free(src);
}

void run_all_to_all_test(int elem_per_thread) {
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

void run_all_reduce_all_test(int elem_per_thread) {
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
  int sz;
  double start_time;
  double stop_time;
  int i;

  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &MYTHREAD);
  MPI_Comm_size(MPI_COMM_WORLD, &THREADS);

  
  switch(argc) {
  case 1: iters=1000; elem_per_thread = 2048; break;
  case 2: iters=atoi(argv[1]); break;
  case 3: iters=atoi(argv[1]); elem_per_thread = atoi(argv[2])/sizeof(double); break;
  default: print_usage(argv[0]); MPI_Finalize(); return 1; break;
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
  
  for(sz = 1; sz<=elem_per_thread; sz*=2) {
    run_bcast_test(sz, 1);
    run_bcast_test(sz, 0);
    run_int_reduce_test(sz, 1);
    run_int_reduce_test(sz, 0);
    run_scatter_test(sz,1);
    run_scatter_test(sz,0);
    run_gather_test(sz, 1);
    run_gather_test(sz, 0);
    run_all_gather_test(sz);
    run_all_to_all_test(sz);

  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
