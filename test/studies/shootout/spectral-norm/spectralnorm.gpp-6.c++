// The Computer Language Benchmarks Game
// http://shootout.alioth.debian.org/
//
// Original C contributed by Sebastien Loisel
// Conversion to C++ by Jon Harrop
// OpenMP parallelize by The Anh Tran

// Fastest with this flag: -Os
// g++ -pipe -Os -fomit-frame-pointer -march=native -fopenmp -mfpmath=sse -msse2 ./spec.c++ -o ./spec.run

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <sched.h>
#include <omp.h>

using namespace std;

static
double eval_A(int i, int j)
{
   // 1.0 / (i + j) * (i + j +1) / 2 + i + 1;
   // n * (n+1) is even number. Therefore, just (>> 1) for (/2)
   int d = (   ( (i + j) * (i + 1 +j) ) >> 1   )    + i + 1;

   return 1.0 / d;
}


static
void eval_A_times_u (const double u[], int inRange, double Au[], int outRange1, int outRange2)
{
   for (int i = outRange1; i < outRange2; i++)
   {
      double sum = 0.0;
      for (int j = 0; j < inRange; j++)
         sum += u[j] * eval_A( i, j );

      Au[i] = sum;
   }
}

static
void eval_At_times_u(const double u[], int inRange, double Au[], int outRange1, int outRange2)
{
   for (int i = outRange1; i < outRange2; i++)
   {
      double sum = 0.0;
      for (int j = 0; j < inRange; j++)
         sum += u[j] * eval_A(j, i);

      Au[i] = sum;
   }
}

static
void eval_AtA_times_u(const double u[], double AtAu[], double v[], int inRange, int range1, int range2)
{
   eval_A_times_u( u, inRange, v, range1, range2 );
   #pragma omp barrier

   eval_At_times_u( v, inRange, AtAu, range1, range2 );
   #pragma omp barrier
}

static
int GetThreadCount()
{
   cpu_set_t cs;
   CPU_ZERO(&cs);
   sched_getaffinity(0, sizeof(cs), &cs);

   int count = 0;
   for (int i = 0; i < 8; i++)
   {
      if (CPU_ISSET(i, &cs))
         count++;
   }
   return count;
}

static
double spectral_game(int N)
{
   __attribute__((aligned(64))) double u[N];
   __attribute__((aligned(64))) double tmp[N];
   __attribute__((aligned(64))) double v[N];

   double vBv   = 0.0;
   double vv   = 0.0;

   // filling 1.0   only 44kB, not parallel worthy

   #pragma omp parallel default(shared) num_threads(GetThreadCount())
   {
      #pragma omp for schedule(static)
      for (int i = 0; i < N; i++)
         u[i] = 1.0;

      // this block will be executed by NUM_THREADS
      // variable declared in this block is private for each thread
      int threadid      = omp_get_thread_num();
      int threadcount   = omp_get_num_threads();
      int chunk   = N / threadcount;

      // calculate each thread's working range [range1 .. range2)   --> static schedule here
      int myRange1 = threadid * chunk;
      int myRange2 = ( threadid < (threadcount -1) ) ? (myRange1 + chunk) : N;

      for (int ite =0; ite < 10; ite++)
      {
         eval_AtA_times_u(u, v, tmp, N, myRange1, myRange2);
         eval_AtA_times_u(v, u, tmp, N, myRange1, myRange2);
      }

      // multi thread adding
      #pragma omp for schedule(static) reduction( + : vBv, vv ) nowait
      for (int i = 0; i < N; i++)
      {
         vv   += v[i] * v[i];
         vBv   += u[i] * v[i];
      }
   } // end parallel region

   return sqrt( vBv/vv );
}


int main(int argc, char *argv[])
{
   int N = ((argc >= 2) ? atoi(argv[1]) : 2000);

   printf("%.9f\n", spectral_game(N));
   return 0;
}

