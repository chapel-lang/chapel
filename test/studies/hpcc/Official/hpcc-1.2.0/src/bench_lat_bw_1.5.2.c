/*
 * Bandwidth-Latency-Benchmark
 *
 * Authors: Rolf Rabenseifner
 *          Gerrit Schulz
 *          Michael Speck
 *
 * Copyright (c) 2003 HLRS, University of Stuttgart
 *
 */

/* -----------------------------------------------------------------------
 *
 * This Bandwidth-Latency-Benchmark measures three types of latency and
 * bandwidth:
 *
 *  - Maximal latency and minimal bandwidth over a set of independently
 *    running ping-pong benchmarks. If there is enough benchmarking time,
 *    then each process makes a ping-pong benchmark with each other
 *    process, otherwise only a subset of process-pairs is used.
 *    (Additionally, minimal and average latency and maximal and average
 *    bandwidth is also reported.)
 *
 *  - Bandwidth per process (and latency) of a ring pattern, i.e.,
 *    each process sends in parallel a message to its neighbor in a ring.
 *    The ring is build by the sequence of the ranks in MPI_COMM_WORLD
 *    (naturally ordered ring).
 *
 *  - Bandwidth and latency of 10 (or 30) different and randomly ordered rings.
 *
 * The major results are:
 *
 *  - maximal ping pong latency,
 *  - average latency of parallel communication in randomly ordered rings,
 *  - minimal ping pong bandwidth,
 *  - bandwidth per process in the naturally ordered ring,
 *  - average bandwidth per process in randomly ordered rings.
 *
 * These five numbers characterize the strength or weakness of a network.
 * For example, the ratio ping pong bandwidth : naturally-ordered-ring
 * bandwidth : random-ring bandwidth, may be
 *  - on a torus network (e.g. T3E):   1 : 1/2 : 1/5
 *  - on a bus connecting n CPUs:      1 : 1/n : 1/n
 *  - on a shared memory vector sys.:  1 : 1/2 : 1/2
 *  - on a full cross-bar:             1 : 1   : 1   (1 MPI process per node)
 *
 * The set of ping-pong measurements is based on an idea of Jack Dongarra
 * communicated with the authors at the EuroPVM/MPI 2003 conference.
 * The ring benchmark is based on the ideas of the effective bandwidth
 * benchmark ( www.hlrs.de/mpi/b_eff ).
 *
 * All measurements are done by repeating the communication pattern
 * several times (see arguments "number_of_measurements") and using
 * the minimal execution time.
 * Each pattern (ping pong, naturally, or randomly ordered ring) is
 * benchmarked by repeating the pattern in a loop (see argument
 * "loop_length") and starting the time measurement (with MPI_Wtime)
 * after a first additional and non-measured iteration and ending
 * the time measurement after the end of this loop.
 * All latency measurements are done with 8 byte messages, and bandwidth
 * measurements with 2,000,000 bytes.
 * Ping pong benchmarking is done with MPI standard send and receive,
 * the ring patterns are communicated in both directions using the
 * best result of two implementations: (a) with two calls to MPI_Sendrecv
 * and (b) with two non-blocking receives and two non-blocking sends
 * (two allow duplex usage of the network links).
 *
 * The benchmarking routine bench_lat_bw() has 2 input arguments:
 *
 *  - The maximal execution time (in seconds) that should be used for
 *    measuring the latency of all ping pong pairs (e.g. on a T3E,
 *    (, and
 *  - the maximal time that should be used for the ping pong bandwidth.
 *
 * Additionally, the benchmark needs 4 GB / random-ring-bandwidth,
 * e.g., 400 sec (40 sec) if the bandwidth is 10 MB/s (100 MB/s),
 * and additionally 3000 * random-ring-latency, e.g., 30 sec (3 sec)
 * if the latency is 10 msec (1 msec).
 *
 * All arguments are in sec or in byte/sec. The printing routine
 * reports on stdout all latency values in milli sec (msec) and
 * all bandwidth values in MB/s (with 1 MB/s = 10**6 byte/sec)
 *
 * -----------------------------------------------------------------------
 *
 * Typical output on a Cray T3E:
 * -----------------------------
 *
 *    ------------------------------------------------------------------
 *    Latency-Bandwidth-Benchmark R1.5 (c) HLRS, University of Stuttgart
 *
 *    Major Benchmark results:
 *    ------------------------
 *
 *    Max Ping Pong Latency:                 0.005209 msecs
 *    Randomly Ordered Ring Latency:         0.007956 msecs
 *    Min Ping Pong Bandwidth:             314.025708 MB/s
 *    Naturally Ordered Ring Bandwidth:    147.600097 MB/s
 *    Randomly  Ordered Ring Bandwidth:     61.096556 MB/s
 *
 *    ------------------------------------------------------------------
 *
 *    Detailed benchmark results:
 *    Ping Pong:
 *    Latency   min / avg / max:   0.004268 /   0.004588 /   0.005209 msecs
 *    Bandwidth min / avg / max:    314.026 /    318.653 /    324.822 MByte/s
 *    Ring:
 *    On naturally ordered ring: latency=      0.008512 msec, bandwidth=    147.600097 MB/s
 *    On randomly  ordered ring: latency=      0.007956 msec, bandwidth=     61.096556 MB/s
 *
 *    ------------------------------------------------------------------
 *
 *    Benchmark conditions:
 *     The latency   measurements were done with        8 bytes
 *     The bandwidth measurements were done with  4000000 bytes
 *     The ring communication was done in both directions on 64 processes
 *     The Ping Pong measurements were done on
 *      -        4032 pairs of processes for latency benchmarking, and
 *      -         462 pairs of processes for bandwidth benchmarking,
 *     out of 64*(64-1) =       4032 possible combinations on 64 processes.
 *     (1 MB/s = 10**6 byte/sec)
 *
 *    ------------------------------------------------------------------
 *
 * Typical output on a NEC SX-5 (shared memory)
 * --------------------------------------------
 *
 *    ------------------------------------------------------------------
 *    Latency-Bandwidth-Benchmark R1.5 (c) HLRS, University of Stuttgart
 *
 *    Major Benchmark results:
 *    ------------------------
 *
 *    Max Ping Pong Latency:                 0.005688 msecs
 *    Randomly Ordered Ring Latency:         0.007819 msecs
 *    Min Ping Pong Bandwidth:            7875.941147 MB/s
 *    Naturally Ordered Ring Bandwidth:   4182.560664 MB/s
 *    Randomly  Ordered Ring Bandwidth:   4393.213906 MB/s
 *
 *    ------------------------------------------------------------------
 *
 *    Detailed benchmark results:
 *    Ping Pong:
 *    Latency   min / avg / max:   0.005595 /   0.005629 /   0.005688 msecs
 *    Bandwidth min / avg / max:   7875.941 /   7912.086 /   7928.861 MByte/s
 *    Ring:
 *    On naturally ordered ring: latency=      0.009812 msec, bandwidth=   4182.560664 MB/s
 *    On randomly  ordered ring: latency=      0.007819 msec, bandwidth=   4393.213906 MB/s
 *
 *    ------------------------------------------------------------------
 *
 *    Benchmark conditions:
 *     The latency   measurements were done with        8 bytes
 *     The bandwidth measurements were done with  4000000 bytes
 *     The ring communication was done in both directions on 6 processes
 *     The Ping Pong measurements were done on
 *      -  30 pairs of processes for latency benchmarking, and
 *      -  30 pairs of processes for bandwidth benchmarking,
 *     out of 6*(6-1) =         30 possible combinations on 6 processes.
 *     (1 MB/s = 10**6 byte/sec)
 *
 *    ------------------------------------------------------------------
 *
 *
 * -----------------------------------------------------------------------
 *
 * Updates
 * - from 1.5.1.0 to 1.5.1.1:  additional arguments in params
 * - bench_lat_bw_1.5.1.1.c = used as hpcc0.6beta/src/bench_lat_bw_1.5.1.c
 * - from 1.5.1.1 to 1.5.1.2:  only pretty print, without any TAB
 * - from 1.5.1.2 to 1.5.2     fixed bug in usage of loop_length_proposal and loop_length
 *
 * ----------------------------------------------------------------------- */

#include <hpcc.h>

/* global vars */
FILE   *OutFile;
double wtick;

#define WTICK_FACTOR 10

/* Message Tags */
#define PING 100
#define PONG 101
#define NEXT_CLIENT 102
#define TO_RIGHT 200
#define TO_LEFT  201

#ifndef CHECK_LEVEL
#  define CHECK_LEVEL 1
#endif

#ifndef DEBUG_LEVEL
#  define DEBUG_LEVEL 2
#endif

typedef struct {
  int    msglen;
  double ring_lat;
  double ring_bwidth;
  double rand_lat;
  double rand_bwidth;
} BenchmarkResult;

/* measurement results, used only on rank 0 */

static void
SumLongLong(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype) {
  int i, n = *len; long long *invecll = (long long *)invec, *inoutvecll = (long long *)inoutvec;
  for (i = n; i; i--, invecll++, inoutvecll++) *inoutvecll += *invecll;
}

/* -----------------------------------------------------------------------
 * Routine: cross_ping_pong_set()
 *
 * Task: PingPong benchmark
 *       to compute minimum, maximum and average latency and bandwidth
 *       over the connections on all (some) pairs of processes.
 *
 * Input:
 *   client_rank_low, client_rank_high, client_rank_stride,
 *   server_rank_low, server_rank_high, server_rank_stride,
 *   flag  -1 = only client_rank  < server_rank
 *          0 = only client_rank != server_rank
 *         +1 = only client_rank  > server_rank
 *   msg_length
 *   loop_length
 *   number_of_measurements
 *
 * Output:
 *   latency_min, latency_avg, latency_max
 *   bandwidth_min, bandwidth_avg, bandwidth_max (if msg_length > 0)
 *   (min, max, avg are done over all pairs of processes)
 *   (after minimum of the latency over all measurements of same pair)
 *
 * Task:
 *
 *   Overview:
 *     - initialization:
 *         Client/Server_rank_low, .._rank_high and .._rank_stride define a set
 *         of client ranks and a set of server ranks.
 *         Client_rank_high is lowered and server_rank_low is enlarged
 *         if a multiple of the strides does not fit.
 *     - execution of the ping-pong benchmarks:
 *         Between each pair of client and server rank (out of the sets)
 *         a ping pong benchmark with a fixed message length is done.
 *         All ping_pong benchmarks are serialized, i.e., never two
 *         process pairs are benchmarked at the same time.
 *         To achieve a minimum of disturbance, each process not involved in
 *         a ping pong must be in the status of waiting for a message.
 *         This principle is fulfilled by sending additional token messages
 *         from a previous client process to the next client process.
 *     - evaluation:
 *         All benchmark results must be stored locally before a total
 *         evaluation can be done, because in the execution sequence,
 *         the iteration over the number_of_measurements is the outer-most
 *         loop, while in the evaluation this loop is the inner-most.
 *
 *   Execution a sequence of the ping pong benchmarks:
 *     for (i_meas=0; i_meas < number_of_measurements; i_meas++)
 *     {
 *       for (client_rank=client_rank_low; client_rank <= client_rank_high; client_rank++client_rank_stride)
 *       {
 *         // the following message receives a token indicating the right to send messages to server processes
 *         if ((myrank == client_rank) && (client_rank > client_rank_low))
 *           MPI_Recv( >>>.... from client_rank-client_rank_stride );
 *         for (server_rank=server_rank_low; server_rank <= server_rank_high; server_rank++server_rank_stride)
 *         {
 *           if ( (flag<0   ? client_rank < server_rank :
 *                 (flag>0  ? client_rank > server_rank : client_rank != server_rank ) ) )
 *           {
 *             PingPongLoop(...);
 *           }
 *         }
 *         // the following message sends a token indicating the right to send messages to server processes
 *         if ((myrank == client_rank) && (client_rank < client_rank_high))
 *           MPI_Send( >>>.... from client_rank+client_rank_stride );
 *         MPI_Bcast( >>> ... root=client_rank_high );
 *       }
 *     }
 *
 *     with PingPongLoop(...)
 *             {
 *               if (myrank == client_rank)
 *               {
 *                 for (i_loop=-1; i_loop < loop_length; i_loop++)
 *                 {
 *                   if (i_loop==0) start_time=MPI_Wtime();
 *                   >>> send ping from client_rank to server_rank
 *                   >>> recv pong from server_rank
 *                 }
 *                 end_time=MPI_Wtime();
 *                 lat_one_meas = end_time-start_time;
 *                 bw_one_meas  = message_length/lat_one_meas;
 *                 >>> store measurement results in the list
 *               }
 *               if (myrank == server_rank)
 *               {
 *                 for (i_loop=-1; i_loop < loop_length; i_loop++)
 *                 {
 *                   >>> recv ping from client_rank
 *                   >>> send pong from server_rank to client_rank
 *                 }
 *               }
 *             }
 *
 *
 *   Evaluation sequence:
 *      latency_min/avg/max
 *       = min/avg/max over all process pairs
 *           of (min over all measurements of lat_one_meas)
 *      bandwidth_min/avg/max
 *       = min/avg/max over all process pairs
 *           of (msg_length / (min over all measurements of lat_one_meas))
 *
 *   Caution: Execution and evaluation sequence are different.
 *            Therefore, each client has to store all measurement results
 *            for all pairs locally, before it can calculate the evaluation
 *            sequence (after all ping-pongs were done).
 *
 * Remarks:
 *   - With using the tokens, there is never a message outstanding
 *     that i not part of a currently running PingPong.
 *   - Processes not involved in a current PingPong are waiting
 *     for the first ping message (if their next role is to be a server process)
 *     or for the token (if their next role is to be a client process).
 *   - At the beginning, a barrier is called
 *   - At the end, the last client process initiates a Bcast.
 *
 * Communication scheme:
 *
 *   Example with client and server rank_low=0, rank_high=11, and rank_stride=3
 *   and flag=0
 *
 *   Rank:    0    1    2    3    4    5    6    7    8    9   10   11
 *
 *   Role:    C              C              C              C
 *                      S              S              S              S
 *
 *   Protcol: ------------------------BARRIER-------------------------
 *            C==<======S
 *            C==<=====================S
 *            C==<====================================S
 *            C==<===================================================S
 *            s------------->r
 *                      S=>==C
 *                           C==<======S
 *                           C==<=====================S
 *                           C==<====================================S
 *                           s------------->r
 *                      S================>==C
 *                                     S=>==C
 *                                          C==<======S
 *                                          C==<=====================S
 *                                          s------------->r
 *                      S===============================>==C
 *                                     S================>==C
 *                                                    S=>==C
 *                                                         C===<=====S
 *            --------------------------------------------Bcast-------
 *
 *    With
 *    --BARRIER--    MPI_Barrier(MPI_COMM_WORLD)
 *    C==<======S    Client Server Ping Pong Loop, with client_rank < server_rank,
 *                   executed only if (flag <= 0)
 *    S======>==C    Client Server Ping Pong Loop, with client_rank > server_rank,
 *                   executed only if (flag >= 0)
 *    -----Bcast-    MPI_Bcast(MPI_COMM_WORLD) with root = last client rank
 *
 * ----------------------------------------------------------------------- */
static
void cross_ping_pong_set(
  int client_rank_low,
  int client_rank_high,
  int client_rank_stride,
  int server_rank_low,
  int server_rank_high,
  int server_rank_stride,
  int msg_length,
  int loop_length,
  int number_of_measurements,
  int flag,
  double *latency_min,
  double *latency_avg,
  double *latency_max,
  double *bandwidth_min,
  double *bandwidth_avg,
  double *bandwidth_max,
  long long *total_number_of_pairs)
{
  MPI_Status status;
  int    client_rank, server_rank;
  int    i_meas;
  int    i_loop, i;
  unsigned char *sndbuf, *rcvbuf;
  double end_time, start_time, lat_one_meas;
  double *local_results;
  double lat, bw;
  int    result_index;
  long long number_of_results;
  int    size, myrank;
  double loc_latency_min;
  double loc_latency_avg;
  double loc_latency_max;
  double loc_bandwidth_min;
  double loc_bandwidth_avg;
  double loc_bandwidth_max;
  MPI_Op sumll;
  int meas_ok;
#if (CHECK_LEVEL >= 1)
  register int base;
#endif

  /* get number of processors and own rank */
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  /* check the benchmark parameter */
  if (client_rank_low < 0) client_rank_low = 0;
  if (client_rank_high >= size) client_rank_high = size-1;
  client_rank_high = (client_rank_high-client_rank_low) /
                      client_rank_stride*client_rank_stride + client_rank_low;
  if (server_rank_low < 0) server_rank_low = 0;
  if (server_rank_high >= size) server_rank_high = size-1;
  server_rank_low = server_rank_high -
        (server_rank_high-server_rank_low)/server_rank_stride*server_rank_stride;

  local_results = (double *) malloc( ((server_rank_high -
        server_rank_low)/server_rank_stride+1) * number_of_measurements *
        sizeof(double) );

  /* set the initial result index*/
  result_index = 0;

  /* get memory for the send/recv buffer */
  sndbuf = (unsigned char *) malloc (msg_length);
  rcvbuf = (unsigned char *) malloc (msg_length);

  number_of_results = 0;

  /* do the measurements */
  for (i_meas=0; i_meas < number_of_measurements; i_meas++)
  {
    result_index = 0;
    for (client_rank=client_rank_low; client_rank <= client_rank_high;
         client_rank += client_rank_stride)
    {
      /* the following message receives a token indicating the right to send
       * messages to server processes
       */
      if ((myrank == client_rank) && (client_rank > client_rank_low))
          MPI_Recv (rcvbuf, 0, MPI_BYTE, client_rank - client_rank_stride,
                    NEXT_CLIENT, MPI_COMM_WORLD, &status);

      /* measurement loop */
      for (server_rank = server_rank_low; server_rank <= server_rank_high;
           server_rank += server_rank_stride)
      {
        if (((flag <= 0) && (server_rank > client_rank)) ||
            ((flag >= 0) && (server_rank < client_rank)))
            {
              if (server_rank==client_rank) fprintf( OutFile, "ALARM\n");
              if (myrank == client_rank)
              {

                do
                {
                  meas_ok = 0;
                  /* communicate loop_length to server_rank */
                  MPI_Send (&loop_length, 1, MPI_INT,
                            server_rank, PING, MPI_COMM_WORLD);

                  for (i_loop = -1; i_loop < loop_length; i_loop++)
                  {
                    if (i_loop == 0) start_time = MPI_Wtime ();
                    /* send ping from client_rank to server_rank */
#if (CHECK_LEVEL >= 1)
                    base = (i_loop + myrank + 1)&0x7f; /* = mod 128 */
                    sndbuf[0] = base; sndbuf[msg_length-1] = base+1;
# if (CHECK_LEVEL >= 2)
                    /* check the check: use a wrong value on process 1 */
                    if (myrank == 1) sndbuf[0] = sndbuf[0] + 11;
# endif
#endif
                    MPI_Send (sndbuf, msg_length, MPI_BYTE,
                              server_rank, PING, MPI_COMM_WORLD);
                    /* recv pong from server_rank */
                    MPI_Recv (rcvbuf, msg_length, MPI_BYTE,
                              server_rank, PONG, MPI_COMM_WORLD, &status);
#if (CHECK_LEVEL >= 1)
                    /* check returned values must be +13 of origin */
                    if (rcvbuf[0] != base+13 || rcvbuf[msg_length-1] != base + 14 )
                    {
                        fprintf( OutFile,  "[%d]: ERROR: expected %u and %u as first and last byte, but got %u and %u instead\n",
                        myrank, base+13, base+14,
                        rcvbuf[0], rcvbuf[msg_length-1] ); fflush( OutFile );
                    }
#endif
                  }
                  end_time = MPI_Wtime ();
                  lat_one_meas = end_time - start_time;

                  if (lat_one_meas < WTICK_FACTOR * wtick)
                  {
                    if (loop_length == 1) loop_length = 2;
                    else loop_length = loop_length * 1.5;
                  }
                  else meas_ok = 1;
                  MPI_Send (&meas_ok, 1, MPI_INT, server_rank, PING, MPI_COMM_WORLD);
                }
                while (!meas_ok);

                /*
                fprintf ( OutFile, "CrossPingPong: Client = %d, Server = %d, "
                        "Latency = %f us \n",
                         client_rank, server_rank,
                        (lat_one_meas * 1e6) / (2 * loop_length)); */
                fflush (OutFile);

                /* workaround to fix problems with MPI_Wtime granularity */
                if (!lat_one_meas)
                {
                  static int complain = 0;
                  lat_one_meas = wtick;
                  if (complain != loop_length)
                  {
#define MSG "In " __FILE__ ", routine bench_lat_bw, the 3rd parameter to cross_ping_pong_controlled was %d; increase it.\n"
                    fprintf (stderr, MSG, loop_length);
                    fprintf (OutFile, MSG, loop_length);
#undef MSG
                  }
                  complain = loop_length;
                }

                /* store measurement results in the list */
                local_results [i_meas*number_of_results + result_index] = lat_one_meas / (loop_length*2);
                result_index++;
              }
              if (myrank == server_rank)
              {
                do
                {
                  meas_ok = 0;
                  /* recv the loop_length from client_rank */
                  MPI_Recv (&loop_length, 1, MPI_INT,
                            client_rank, PING, MPI_COMM_WORLD, &status);

                  for (i_loop = -1; i_loop < loop_length; i_loop++)
                  {
                    /* recv ping from client_rank */
                    MPI_Recv (rcvbuf, msg_length, MPI_BYTE,
                              client_rank, PING,
                              MPI_COMM_WORLD, &status);

#if (CHECK_LEVEL >= 1)
                    /* server returns received value + const */
                    sndbuf[0] =             rcvbuf[0] + 13;
                    sndbuf[msg_length-1] =  rcvbuf[msg_length-1] + 13;
# if (CHECK_LEVEL >= 2)
                    /* check the check: use a wrong value on process 1 */
                    if (myrank == 1) sndbuf[msg_length-1] = sndbuf[msg_length-1] + 22;
# endif
#endif

                    /* send pong from server_rank to client_rank */
                    MPI_Send (sndbuf, msg_length, MPI_BYTE, client_rank, PONG,
                              MPI_COMM_WORLD);

                  }
                  MPI_Recv (&meas_ok, 1, MPI_INT, client_rank, PING,
                            MPI_COMM_WORLD, &status);
                }
                while(!meas_ok);
              }
            }
      }

      /* the following message sends a token indicating the right to send
       * messages to server processes
       */
      if ((myrank == client_rank) && (client_rank < client_rank_high))
          MPI_Send (sndbuf, 0, MPI_BYTE, client_rank + client_rank_stride,
                    NEXT_CLIENT, MPI_COMM_WORLD);

      MPI_Bcast (sndbuf, 0, MPI_BYTE, client_rank_high, MPI_COMM_WORLD);
    }
    number_of_results = result_index;
  }

  /* free the send/recv buffer */
  free (sndbuf);
  free (rcvbuf);

  /* compute local min, max and avg on all client processes */
  /* gather minimal latency for all indexes in first measurement of all measurements */
  for ( i = 0; i < number_of_results; i++ )
    for (i_meas = 1; i_meas < number_of_measurements; i_meas++)
      if ( local_results[i_meas*number_of_results+i] < local_results[i] )
        local_results[i] = local_results[i_meas*number_of_results+i];

  loc_latency_min = 1e99;
  loc_latency_avg = 0;
  loc_latency_max = 0;
  loc_bandwidth_min = 1e99;
  loc_bandwidth_avg = 0;
  loc_bandwidth_max = 0;
  for (i=0; i < number_of_results; i++)
  {
    lat = local_results[i];  bw = msg_length / lat;
#if (DEBUG_LEVEL >= 3)
    if ((myrank == 0) || (DEBUG_LEVEL >= 4)) {
      fprintf ( OutFile, "[%d] i=%d, lat=%10.6fms, bw=%10.6fMB/s\n", myrank, i, lat*1e3, bw/1e6); fflush( OutFile );
    }
#endif
    if (lat < (loc_latency_min))  loc_latency_min = lat;
    loc_latency_avg = loc_latency_avg + lat;
    if (lat > (loc_latency_max))  loc_latency_max = lat;
    if (bw < (loc_bandwidth_min))  loc_bandwidth_min = bw;
    loc_bandwidth_avg = loc_bandwidth_avg + bw;
    if (bw > (loc_bandwidth_max))  loc_bandwidth_max = bw;
  }
#if (DEBUG_LEVEL >= 3)
  if ((myrank == 0) || (DEBUG_LEVEL >= 4)) {
    fprintf ( OutFile, "[%d] Latency   min / avg / max: %10.6f / %10.6f / %10.6f msecs\n",
              myrank, loc_latency_min * 1e3, loc_latency_avg / number_of_results * 1e3, loc_latency_max * 1e3);  fflush( OutFile );
    fprintf ( OutFile, "[%d] Bandwidth min / avg / max: %10.3f / %10.3f / %10.3f MByte/s\n\n",
              myrank, loc_bandwidth_min / 1e6, loc_bandwidth_avg / number_of_results / 1e6, loc_bandwidth_max / 1e6);  fflush( OutFile );
  }
#endif

  /* free the local result list */
  free (local_results);

  /* send all local results to process 0 */
  MPI_Op_create( SumLongLong, 1, &sumll );
  MPI_Reduce (&number_of_results, total_number_of_pairs, 1, MPI_LONG_LONG_INT, sumll, 0,
              MPI_COMM_WORLD);
  MPI_Op_free( &sumll );
  MPI_Reduce (&loc_latency_min, latency_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce (&loc_latency_avg, latency_avg, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce (&loc_latency_max, latency_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce (&loc_bandwidth_min, bandwidth_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce (&loc_bandwidth_avg, bandwidth_avg, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce (&loc_bandwidth_max, bandwidth_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  /* compute global average on process 0 */
  if ((myrank == 0) && (*total_number_of_pairs > 0))
  {
    *latency_avg= *latency_avg / (*total_number_of_pairs);
    *bandwidth_avg= *bandwidth_avg / (*total_number_of_pairs);
  }

#if (DEBUG_LEVEL >= 2)
  /* print the results */
    if (myrank == 0)
    {
      fprintf ( OutFile, "Message Length: %d\n", msg_length);
      fprintf ( OutFile, "Latency   min / avg / max: %10.6f / %10.6f / %10.6f msecs\n",
              *latency_min * 1e3, *latency_avg * 1e3, *latency_max * 1e3);
      fprintf ( OutFile, "Bandwidth min / avg / max: %10.3f / %10.3f / %10.3f MByte/s\n\n",
              *bandwidth_min / 1e6, *bandwidth_avg / 1e6, *bandwidth_max / 1e6);
      fflush( OutFile );
    }
#endif
}

/* -----------------------------------------------------------------------
 * Routine: cross_ping_pong_controlled()
 *
 * Task: Choose a set of input arguments for PingPongSet
 *       to benchmark the minimal/average/maximal latency and
 *       bandwidth of a system based on a given amount of time.
 *
 * Input:
 *   max_time, msg_length, loop_length, number_of_measurements
 *
 * Output:
 *   latency_min,   latency_avg,   latency_max
 *   bandwidth_min, bandwidth_avg, bandwidth_max
 *   (min, max, avg are done over all pairs of processes)
 *
 * Execution task:
 *   - benchmarking latency and bandwidth for msg_length byte
 *     of communication with client_rank=0 and server_rank=size-1
 *   - calculating client and server rank_stride to guarantee, that
 *     -  PingPongSet does not need more than max_time sec
 * ----------------------------------------------------------------------- */
static
void cross_ping_pong_controlled(
  double max_time,
  int    msg_length,
  int    loop_length,
  int    number_of_measurements,
  double *latency_min,
  double *latency_avg,
  double *latency_max,
  double *bandwidth_min,
  double *bandwidth_avg,
  double *bandwidth_max,
  long long *number_of_pairs
)
{
  int    size, myrank, i;
  double l_dum_min, l_dum_max; /* dummies */
  double b_dum_min, b_dum_avg, b_dum_max; /* dummies */
  long long dum_num_results; /* dummies */
  int    stride;
  double lat_msg;
  int    max_pings, not_prime;
  long long max_pairs;

  /* basic MPI initialization */
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  cross_ping_pong_set( 0,0,1,  size-1,size-1,1,
                    msg_length, loop_length, number_of_measurements, 0,
                    &l_dum_min,  &lat_msg,  &l_dum_max,
                    &b_dum_min,  &b_dum_avg,  &b_dum_max, &dum_num_results);

  if ( myrank == 0 ) {
    if (lat_msg*2*(loop_length+1) >= WTICK_FACTOR*wtick)
    {
      max_pairs = max_time / (lat_msg*2*(loop_length+1)*number_of_measurements);
      fprintf( OutFile,  "MPI_Wtime granularity is ok.\n");
    }
    else
    {
      max_pairs = max_time / (WTICK_FACTOR*wtick*number_of_measurements);
      fprintf( OutFile,  "Use MPI_Wtick for estimation of max pairs\n");
      fflush( OutFile );
    }
    max_pings = (int)sqrt( (double)max_pairs );
    if ( max_pings < 5 ) max_pings = 5;
    stride = 1.0 * size / max_pings + 0.9;
    if ( stride < 1 ) stride = 1;
    if ( stride == 2) stride = 3;
    if ( stride > 3 ) {
      while ( 1 ) {
        not_prime = 0;
        for ( i = 2;  i < stride; i++ )
          if ( (stride % i) == 0 ) {
            not_prime = 1;
            break;
          }
        if ( not_prime )
          if ( stride > (size/3) ) break;
          else stride++;
        else
          break;
      }
    }
#if (DEBUG_LEVEL >= 2)
    fprintf( OutFile,  "message size:                         %10d\n", msg_length );
    fprintf( OutFile,  "max time :                            %10.6f secs\n", max_time );
    fprintf( OutFile,  "latency for msg:                      %10.6f msecs\n", lat_msg*1e3 );
    fprintf( OutFile,  "estimation for ping pong:             %10.6f msecs\n", lat_msg*2*(loop_length+1)*number_of_measurements*1e3);
    fprintf( OutFile,  "max number of ping pong pairs       = %10.0f\n", 1.0*max_pairs );
    fprintf( OutFile,  "max client pings = max server pongs = %10d\n", max_pings );
    fprintf( OutFile,  "stride for latency                  = %10d\n", stride );
    fflush( OutFile );
#endif
  }
  MPI_Bcast ( &stride, 1, MPI_INT, 0, MPI_COMM_WORLD);
  cross_ping_pong_set( 0, size-1, stride, 0, size-1, stride,
                    msg_length, loop_length, number_of_measurements, 0,
                    latency_min, latency_avg, latency_max,
                    bandwidth_min, bandwidth_avg, bandwidth_max, number_of_pairs);
}

/* -----------------------------------------------------------------------
 * Routine: ring_lat_bw_loop()
 *
 *
 * Task: Communicate to left and right partner in rand_pattern_count
 *       random rings and the naturally ordered ring. Reduce the maximum
 *       of all measurements over all processors to rank 0 and get the
 *       minimal measurement on it. Compute naturally ordered and avg
 *       randomly ordered latency and bandwidth.
 *
 * Input:
 *   msglen, measurements, loop_length, rand_pattern_count
 *
 * Output:
 *   result->msglen, result->ring_lat, result->rand_lat,
 *   result->ring_bwidth, result->rand_bwidth
 *
 * Execution Tasks:
 *
 * - loop loop_length * measurements times and do Irecv,Isend to left
 *   and right partner as well as Sendrecv and save the minimum of both
 *   latencies for all rings.
 * - Reduce all measurements*(rand_pattern_count+1) latencies to rank 0
 *   and get minimal measurement on it.
 * - Compute latencies and bandwidth. For random order the geometric average
 * of the latency is built.
 * ----------------------------------------------------------------------- */
static
void ring_lat_bw_loop(
  int msglen,
  int measurements,
  int loop_length_proposal,
  int rand_pattern_count,
  BenchmarkResult *result )
{
  int i_meas, i_pat, i_loop, i, j;
  double start_time, end_time, lat_sendrecv, lat_nonblocking;
  double *latencies; /* measurements * (rand_pattern_count+1) */
  double *max_latencies; /* reduced from all processors with MPI_MAX on rank 0 */
  double avg_latency; /* of random pattern rings */
  int *ranks; /* communication pattern, order of processors */
  int size, myrank, left_rank, right_rank;
  MPI_Request requests[4];
  MPI_Status statuses[4];
  unsigned char *sndbuf_left, *sndbuf_right, *rcvbuf_left, *rcvbuf_right;
  long seedval;
  double rcp = 1.0 / RAND_MAX;
  int loop_length;
  int meas_ok, meas_ok_recv;
#if (CHECK_LEVEL >= 1)
  register int base;
#endif

  /* get number of processors and own rank */
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  /* alloc memory and init with 0 */
  latencies     = (double *)malloc( measurements * (rand_pattern_count+1) * sizeof( *latencies ) );
  max_latencies = (double *)malloc( measurements * (rand_pattern_count+1) * sizeof( *max_latencies ) );
  ranks = (int *)malloc( size * sizeof( *ranks ) );
  sndbuf_left  = (unsigned char *)malloc( msglen );
  sndbuf_right = (unsigned char *)malloc( msglen );
  rcvbuf_left  = (unsigned char *)malloc( msglen );
  rcvbuf_right = (unsigned char *)malloc( msglen );

  /* init pseudo-random with time seed */
  seedval=(long)(time((time_t *) 0));
#if (DEBUG_LEVEL >= 3)
  if (myrank==0) { fprintf( OutFile, "seedval = %ld\n",seedval); fflush( OutFile ); }
#endif

  /* benchmark */
  for ( i_meas = 0; i_meas < measurements; i_meas++ ) {
    srand(seedval);
    for ( i_pat = 0; i_pat < rand_pattern_count+1; i_pat++ ) {
      /* build pattern at rank 0 and broadcast to all */
      if ( myrank == 0 ) {
        if (i_pat>0) { /* random pattern */
          for (i=0; i<size; i++) ranks[i] = -1;
          for (i=0; i<size; i++) {
            j = (int)(rand() * rcp * size);
            while (ranks[j] != -1) j = (j+1) % size;
            ranks[j] = i;
          }
        }
        else { /* naturally ordered ring */
          for (i=0; i<size; i++) ranks[i] = i;
        }
#if (DEBUG_LEVEL >= 3)
        if ( i_meas == 0 ) {
          fprintf( OutFile, "i_pat=%3d: ",i_pat);
          for (i=0; i<size; i++) fprintf( OutFile, " %2d",ranks[i]);
          fprintf( OutFile,  "\n" );  fflush( OutFile );
        }
#endif
      }
      MPI_Bcast(ranks, size, MPI_INT, 0, MPI_COMM_WORLD);

      /* get rank of left and right partner. therefore find myself (myrank)
       * in pattern first. */
      for ( i = 0; i < size; i++ )
        if ( ranks[i] == myrank ) { /* will definitely be found */
          left_rank = ranks[(i-1+size)%size];
          right_rank = ranks[(i+1)%size];
        }

      do
      {
        meas_ok = 0;
        MPI_Allreduce (&loop_length_proposal, &loop_length, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        loop_length_proposal = loop_length;

        /* loop communication */
        for ( i_loop = -1; i_loop < loop_length; i_loop++ ) {
          if ( i_loop == 0 ) start_time = MPI_Wtime();

          /* communicate to left and right partner */
#if (CHECK_LEVEL >= 1)
          base = (i_loop + myrank + 1)&0x7f; /* = mod 128 */
          sndbuf_right[0] = base; sndbuf_right[msglen-1] = base+1;
          sndbuf_left[0]  = base+2; sndbuf_left[msglen-1]  = base+3;
# if (CHECK_LEVEL >= 2)
          /* check the check: use a wrong value on process 1 */
          if (myrank == 1) sndbuf_right[0] = sndbuf_right[0] + 33;
          if (myrank == 1) sndbuf_left[msglen-1] = sndbuf_left[msglen-1] + 44;
# endif
#endif
          MPI_Sendrecv(
            sndbuf_right, msglen, MPI_BYTE,
            right_rank, TO_RIGHT,
            rcvbuf_left, msglen, MPI_BYTE,
            left_rank, TO_RIGHT,
            MPI_COMM_WORLD, &(statuses[0]) );
          MPI_Sendrecv(
            sndbuf_left, msglen, MPI_BYTE,
            left_rank, TO_LEFT,
            rcvbuf_right, msglen, MPI_BYTE,
            right_rank, TO_LEFT,
            MPI_COMM_WORLD, &(statuses[1]) );
#if (CHECK_LEVEL >= 1)
          /* check whether bytes are received correctly */
          base = (i_loop + left_rank + 1)&0x7f; /* = mod 128 */
          if ( rcvbuf_left[0] != base || rcvbuf_left[msglen-1] != base+1 )
          {
            fprintf( OutFile,  "[%d]: ERROR: from right: expected %u and %u as first and last byte, but got %u and %u instead\n",
            myrank, base, base+1,
            rcvbuf_left[0], rcvbuf_left[msglen-1] ); fflush( OutFile );
          }
          base = (i_loop + right_rank + 1)&0x7f; /* = mod 128 */
          if ( rcvbuf_right[0] != base+2 || rcvbuf_right[msglen-1] != base + 3 )
          {
            fprintf( OutFile,  "[%d]: ERROR: from right: expected %u and %u as first and last byte, but got %u and %u instead\n",
            myrank, base+2, base+3,
            rcvbuf_right[0], rcvbuf_right[msglen-1] ); fflush( OutFile );
          }
#endif
        }
        end_time = MPI_Wtime();
        if ((end_time-start_time) < WTICK_FACTOR * wtick)
        {
          if (loop_length_proposal == 1) loop_length_proposal = 2;
          else loop_length_proposal = loop_length_proposal * 1.5;
        }
        else meas_ok=1;
        MPI_Allreduce (&meas_ok, &meas_ok_recv, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        meas_ok = meas_ok_recv;
      }
      while (!meas_ok);
      lat_sendrecv = (end_time-start_time) / (2 * loop_length);

      /* communication loop with non-blocking routines, and previous loop_length */
      for ( i_loop = -1; i_loop < loop_length; i_loop++ ) {
        if ( i_loop == 0 ) start_time = MPI_Wtime();
#if (CHECK_LEVEL >= 1)
        /* communicate to left and right partner */
        base = (i_loop + myrank + 1)&0x7f; /* = mod 128 */
        sndbuf_right[0] = base; sndbuf_right[msglen-1] = base+1;
        sndbuf_left[0]  = base+2; sndbuf_left[msglen-1]  = base+3;
#endif
        /* irecv left */
        MPI_Irecv(
          rcvbuf_left, msglen, MPI_BYTE,
          left_rank, TO_RIGHT,
          MPI_COMM_WORLD, &requests[0] );
        /* irecv right */
        MPI_Irecv(
          rcvbuf_right, msglen, MPI_BYTE,
          right_rank, TO_LEFT,
          MPI_COMM_WORLD, &requests[1] );
        /* isend right */
        MPI_Isend(
          sndbuf_right, msglen, MPI_BYTE,
          right_rank, TO_RIGHT,
          MPI_COMM_WORLD, &requests[2] );
        /* isend left */
        MPI_Isend(
          sndbuf_left, msglen, MPI_BYTE,
          left_rank, TO_LEFT,
          MPI_COMM_WORLD, &requests[3] );
        /* waitall */
        MPI_Waitall( 4, requests, statuses );
#if (CHECK_LEVEL >= 1)
        /* check whether both transfers were done right */
        base = (i_loop + left_rank + 1)&0x7f; /* = mod 128 */
        if ( rcvbuf_left[0] != base || rcvbuf_left[msglen-1] != base+1 )
        {
          fprintf( OutFile,  "[%d]: ERROR: from right: expected %u and %u as first and last byte, but got %u and %u instead\n",
          myrank, base, base+1,
          rcvbuf_left[0], rcvbuf_left[msglen-1] ); fflush( OutFile );
        }
        base = (i_loop + right_rank + 1)&0x7f; /* = mod 128 */
        if ( rcvbuf_right[0] != base+2 || rcvbuf_right[msglen-1] != base + 3 )
        {
          fprintf( OutFile,  "[%d]: ERROR: from right: expected %u and %u as first and last byte, but got %u and %u instead\n",
          myrank, base+2, base+3,
          rcvbuf_right[0], rcvbuf_right[msglen-1] ); fflush( OutFile );
        }
#endif
      }
      end_time = MPI_Wtime();
      lat_nonblocking = (end_time-start_time) / ( 2 * loop_length );

      /* workaround to fix problems with MPI_Wtime granularity */
      if (!lat_nonblocking)
      {
        static int complain = 0;
        lat_nonblocking = wtick;
        if (complain != loop_length)
        {
#define MSG "In " __FILE__ ", routine bench_lat_bw, the 3rd parameter to ring_lat_bw_loop was %d; increase it.\n"
          fprintf( stderr, MSG, loop_length);
          fprintf( OutFile, MSG, loop_length);
#undef MSG
        }
        complain = loop_length;
      }

      latencies[i_meas*(rand_pattern_count+1)+i_pat] =
      (lat_sendrecv < lat_nonblocking ? lat_sendrecv : lat_nonblocking);
    }
  }

#if (DEBUG_LEVEL >= 5)
  if ((myrank == 0) || (DEBUG_LEVEL >= 6)) {
    fprintf( OutFile,  "RANK %3d: ", myrank );
    for ( i = 0; i < measurements*(rand_pattern_count+1); i++ )
      fprintf( OutFile,  "%e  ", latencies[i] );
    fprintf( OutFile,  "\n" ); fflush( OutFile );
  }
#endif

  /* reduce all vectors to get maximum vector at rank 0 */
  MPI_Reduce(
    latencies, max_latencies,
    measurements * (rand_pattern_count+1), MPI_DOUBLE,
    MPI_MAX, 0, MPI_COMM_WORLD );

#if (DEBUG_LEVEL >= 5)
       fflush(stdout);
       MPI_Barrier(MPI_COMM_WORLD);
       if (myrank==0)
       {
         fprintf( OutFile,  "RANK ---: " );
         for ( i = 0; i < measurements*(rand_pattern_count+1); i++ )
           fprintf( OutFile,  "%e  ", max_latencies[i] );
         fprintf( OutFile,  "\n" ); fflush( OutFile );
       }
#endif

  /* get minimal measurement from vector as final measurement and compute latency and bandwidth */
  if ( myrank == 0 ) {
    /* reduce measurements to first minimal measurement */
    for ( i_pat = 0; i_pat < rand_pattern_count+1; i_pat++ )
    {
      for (i_meas = 1; i_meas < measurements; i_meas++)
      { /* minimal latencies over all measurements */
        if (max_latencies[i_meas*(rand_pattern_count+1)+i_pat] < max_latencies[i_pat])
          max_latencies[i_pat] = max_latencies[i_meas*(rand_pattern_count+1)+i_pat];
      }
    }

    /* get average latency of random rings by geometric means */
    avg_latency = 0;
    for ( i_pat = 1; i_pat < rand_pattern_count+1; i_pat++ )
    avg_latency += log( max_latencies[i_pat] );
    avg_latency = avg_latency / rand_pattern_count;
    avg_latency = exp( avg_latency );

    /* compute final benchmark results */
    result->msglen = msglen;
    result->ring_lat = max_latencies[0];
    result->ring_bwidth = msglen / max_latencies[0];
    result->rand_lat = avg_latency;
    result->rand_bwidth = msglen / avg_latency;
  }

  /* free memory */
  free( ranks );
  free( latencies );
  free( max_latencies );
  free(sndbuf_left);
  free(sndbuf_right);
  free(rcvbuf_left);
  free(rcvbuf_right);
#if (DEBUG_LEVEL >= 2)
   if (myrank == 0)
   {
     fprintf( OutFile,  "Message Size:               %13d Byte\n",   result->msglen );
     fprintf( OutFile,  "Natural Order Latency:      %13.6f msec\n", result->ring_lat*1e3 );
     fprintf( OutFile,  "Natural Order Bandwidth:    %13.6f MB/s\n", result->ring_bwidth/1e6 );
     fprintf( OutFile,  "Avg Random Order Latency:   %13.6f msec\n", result->rand_lat*1e3 );
     fprintf( OutFile,  "Avg Random Order Bandwidth: %13.6f MB/s\n", result->rand_bwidth/1e6 );
     fprintf( OutFile,  "\n" );
     fflush( OutFile );
   }
#endif
}

/* -----------------------------------------------------------------------
 * Routine: bench_lat_bw()
 *
 * Task: Run cross_ping_pong_controlled and ring_lat_bw_loop
 *       with a well chosen number of loops and measurements
 *       to benchmark the minimal/average/maximal latency and
 *       bandwidth of a system based on a given amount of time.
 *
 * Input:
 *   max_time_for_latency, max_time_for_bandwidth
 *
 * Output:
 *   msg_length_for_lat, msg_length_for_bw,
 *   latency_min,   latency_avg,   latency_max,
 *   bandwidth_min, bandwidth_avg, bandwidth_max,
 *   number_of_pairs_for_lat, number_of_pairs_for_bw,
 *   (min, max, avg are done over all pairs of processes for
 *    ping pong benchmarking)
 *   ring_lat, ring_bw, rand_lat, rand_bw
 *   (for ring benchmarking)
 *
 * Execution Tasks:
 *   - run cross_ping_pong_controlled for 8 and 2000000 bytes
 *   - run ring_lat_bw_loop for 8 and 2000000 bytes
 *   - use the results from message length 8 byte for latency
 *     and the results from message length 2000000 for bandwidth
 * ----------------------------------------------------------------------- */
static
void bench_lat_bw(
  double max_time_for_latency,   /* for ping pong */
  double max_time_for_bandwidth, /* for ping pong */
  int    *msg_length_for_lat,
  int    *msg_length_for_bw,
  double *latency_min, /* */
  double *latency_avg, /* ping pong measurement latency */
  double *latency_max, /* */
  long long *number_of_pairs_for_lat, /* ping pong */
  double *bandwidth_min, /* */
  double *bandwidth_avg, /* ping pong measurement bandwidth */
  double *bandwidth_max, /* */
  long long *number_of_pairs_for_bw, /* ping pong */
  double *ring_lat, /* naturally ordered ring latency */
  double *rand_lat, /* randomly  ordered ring latency */
  double *ring_bw,  /* randomly  ordered ring bandwidth */
  double *rand_bw  /* naturally ordered ring bandwidth */
)
{
  double l_dum_min, l_dum_avg, l_dum_max; /* dummies */
  double b_dum_min, b_dum_avg, b_dum_max; /* dummies */
  BenchmarkResult result_lat, result_bw;
  double wtick_recv;
# if (DEBUG_LEVEL >= 1)
  int size, myrank;
  double wtime_total, wtime_cross_lat, wtime_cross_bw, wtime_ring_lat, wtime_ring_bw;
# endif

  *msg_length_for_lat = 8;
  *msg_length_for_bw  = 2000000;

# if (DEBUG_LEVEL >= 1)
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
# endif

  /* get the granularity of MPI_Wtime, but don't trust MPI_Wtick!! */
  wtick = MPI_Wtick();
# ifdef SET_WTICK
    wtick = SET_WTICK ;
# endif
  if (wtick < 0) wtick = -wtick;
  MPI_Allreduce (&wtick, &wtick_recv, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  wtick = wtick_recv;
# if (DEBUG_LEVEL >= 1)
  if (myrank == 0)
  {
    fprintf( OutFile, "MPI_Wtime granularity.\n");
    fprintf( OutFile, "Max. MPI_Wtick is %f sec\n", wtick);
  }
# endif
  if (wtick < 1e-6) wtick = 1e-6;
  if (wtick > 0.01) wtick = 0.01;
# if (DEBUG_LEVEL >= 1)
  if (myrank == 0)
  {
    fprintf( OutFile, "wtick is set to   %f sec  \n\n", wtick);
    fflush( OutFile );
  }
# endif

  /* ping pong */
  /* --------- */

# if (DEBUG_LEVEL >= 1)
    wtime_total     = - MPI_Wtime();
    wtime_cross_lat = - MPI_Wtime();
# endif

  cross_ping_pong_controlled( max_time_for_latency, *msg_length_for_lat, 8, 5,
                           latency_min, latency_avg, latency_max,
                           &b_dum_min, &b_dum_avg, &b_dum_max,
                           number_of_pairs_for_lat );

# if (DEBUG_LEVEL >= 1)
    wtime_cross_lat +=  MPI_Wtime();
    wtime_cross_bw  = - MPI_Wtime();
# endif

  cross_ping_pong_controlled( max_time_for_bandwidth, *msg_length_for_bw, 1, 2,
                           &l_dum_min, &l_dum_avg, &l_dum_max,
                           bandwidth_min, bandwidth_avg, bandwidth_max,
                           number_of_pairs_for_bw );

# if (DEBUG_LEVEL >= 1)
    wtime_cross_bw  +=  MPI_Wtime();
# endif

  /* ring */

# if (DEBUG_LEVEL >= 1)
    wtime_ring_lat = - MPI_Wtime();
# endif

  ring_lat_bw_loop( *msg_length_for_lat, 8, 5, 30, &result_lat );
  *ring_lat = result_lat.ring_lat;
  *rand_lat = result_lat.rand_lat;

# if (DEBUG_LEVEL >= 1)
    wtime_ring_lat +=  MPI_Wtime();
    wtime_ring_bw  = - MPI_Wtime();
# endif

  ring_lat_bw_loop( *msg_length_for_bw,  3, 2, 10, &result_bw );
  *ring_bw = result_bw.ring_bwidth;
  *rand_bw = result_bw.rand_bwidth;

# if (DEBUG_LEVEL >= 1)
    wtime_ring_bw  +=  MPI_Wtime();
    wtime_total    +=  MPI_Wtime();
# endif

# if (DEBUG_LEVEL >= 1)
    if (myrank==0)
    { fprintf( OutFile, "Execution time (wall clock)      = %9.3f sec on %d processes\n", wtime_total, size);
      fprintf( OutFile, " - for cross ping_pong latency   = %9.3f sec\n", wtime_cross_lat);
      fprintf( OutFile, " - for cross ping_pong bandwidth = %9.3f sec\n", wtime_cross_bw );
      fprintf( OutFile, " - for ring latency              = %9.3f sec\n", wtime_ring_lat);
      fprintf( OutFile, " - for ring bandwidth            = %9.3f sec\n", wtime_ring_bw );
      fflush( OutFile );
    }
# endif
}

/* -----------------------------------------------------------------------
 * Routine: bench_lat_bw_print()
 *
 * Task: Print out the benchmark results and conditions from
 *       bench_lat_bw.
 *
 * Input:
 *  none
 *
 * Output:
 *   none
 *
 * Execution Tasks:
 *   - run bench_lat_bw
 *   - print out the five most important values:
 *      - max ping pong latency
 *      - min ping pong bandwidth
 *      - randomly ordered ring latency
 *      - naturally ordered ring bandwidth
 *      - randomly ordered ring bandwidth
 *   - print all benchmark results on debug level 1
 *   - print benchmark conditions:
 *     - number of processors
 *     - message lengths
 *     - number of ping pong pairs
 * ----------------------------------------------------------------------- */
static
void bench_lat_bw_print(double *MaxPingPongLatency, double *RandomlyOrderedRingLatency,
  double *MinPingPongBandwidth, double *NaturallyOrderedRingBandwidth,
  double *RandomlyOrderedRingBandwidth,
  double *MinPingPongLatency, double *AvgPingPongLatency, double *MaxPingPongBandwidth,
  double *AvgPingPongBandwidth, double *NaturallyOrderedRingLatency) {
  int msg_length_for_lat;
  int msg_length_for_bw;
  double ring_lat, rand_lat;
  double ring_bw,  rand_bw;
  int size, myrank;
  double max_time_for_latency;
  double max_time_for_bandwidth;
  double latency_min;
  double latency_avg;
  double latency_max;
  double bandwidth_min;
  double bandwidth_avg;
  double bandwidth_max;
  long long number_of_pairs_for_lat, number_of_pairs_for_bw;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
# if (DEBUG_LEVEL >= 1)
    if (myrank == 0 )
    {
      fprintf ( OutFile, "\n------------------------------------------------------------------\n");
      fprintf ( OutFile, "Latency-Bandwidth-Benchmark R1.5.1 (c) HLRS, University of Stuttgart\n");
      fprintf ( OutFile, "Written by Rolf Rabenseifner, Gerrit Schulz, and Michael Speck, Germany\n\n");
      fprintf ( OutFile, "Details - level %d\n", DEBUG_LEVEL);
      fprintf ( OutFile, "-----------------\n\n");
      fflush( OutFile );
    }
# endif

  /* The following timings are used for the cross ping pong.
     Additionally, about 300 seconds (on a 100 MB/s) are necessary
     for benchmarking the ring patterns. */
  max_time_for_latency   = 10.0 /*sec*/;
  max_time_for_bandwidth = 30.0 /*sec*/;
  bench_lat_bw( max_time_for_latency, max_time_for_bandwidth,
                &msg_length_for_lat, &msg_length_for_bw,
                &latency_min, &latency_avg, &latency_max,
                &number_of_pairs_for_lat,
                &bandwidth_min, &bandwidth_avg, &bandwidth_max,
                &number_of_pairs_for_bw,
                &ring_lat, &rand_lat, &ring_bw, &rand_bw );

  if (myrank == 0 )
  {
      fprintf ( OutFile, "\n------------------------------------------------------------------\n");
      fprintf ( OutFile, "Latency-Bandwidth-Benchmark R1.5.1 (c) HLRS, University of Stuttgart\n");
      fprintf ( OutFile, "Written by Rolf Rabenseifner, Gerrit Schulz, and Michael Speck, Germany\n\n");

      fprintf( OutFile,  "Major Benchmark results:\n" );
      fprintf( OutFile,  "------------------------\n\n" );
      fprintf( OutFile,  "Max Ping Pong Latency:            %13.6f msecs\n", latency_max*1e3 );
      fprintf( OutFile,  "Randomly Ordered Ring Latency:    %13.6f msecs\n", rand_lat*1e3 );
      fprintf( OutFile,  "Min Ping Pong Bandwidth:          %13.6f MB/s\n", bandwidth_min/1e6 );
      fprintf( OutFile,  "Naturally Ordered Ring Bandwidth: %13.6f MB/s\n", ring_bw/1e6 );
      fprintf( OutFile,  "Randomly  Ordered Ring Bandwidth: %13.6f MB/s\n", rand_bw/1e6 );
      *MaxPingPongLatency = latency_max * 1e6;      /* usec */
      *RandomlyOrderedRingLatency = rand_lat * 1e6; /* usec */
      *MinPingPongBandwidth = bandwidth_min * 1e-9;    /* GB/s */
      *NaturallyOrderedRingBandwidth = ring_bw * 1e-9; /* GB/s */
      *RandomlyOrderedRingBandwidth = rand_bw * 1e-9;  /* GB/s */
      *MinPingPongLatency = latency_min * 1e6;      /* usec */
      *AvgPingPongLatency = latency_avg * 1e6;      /* usec */
      *MaxPingPongBandwidth = bandwidth_max * 1e-9;    /* GB/s */
      *AvgPingPongBandwidth = bandwidth_avg * 1e-9;    /* GB/s */
      *NaturallyOrderedRingLatency = ring_lat * 1e6; /* usec */


      fprintf ( OutFile, "\n------------------------------------------------------------------\n");

      fprintf( OutFile,  "\nDetailed benchmark results:\n" );
      fprintf( OutFile,  "Ping Pong:\n" );
      fprintf ( OutFile, "Latency   min / avg / max: %10.6f / %10.6f / %10.6f msecs\n",
               latency_min*1e3, latency_avg*1e3, latency_max*1e3);
      fprintf ( OutFile, "Bandwidth min / avg / max: %10.3f / %10.3f / %10.3f MByte/s\n",
               bandwidth_min/1e6, bandwidth_avg/1e6, bandwidth_max/1e6);
      fprintf( OutFile,  "Ring:\n" );
      fprintf( OutFile,  "On naturally ordered ring: latency= %13.6f msec, bandwidth= %13.6f MB/s\n", ring_lat*1e3, ring_bw/1e6);
      fprintf( OutFile,  "On randomly  ordered ring: latency= %13.6f msec, bandwidth= %13.6f MB/s\n", rand_lat*1e3, rand_bw/1e6);

      fprintf ( OutFile, "\n------------------------------------------------------------------\n");

      fprintf( OutFile,  "\nBenchmark conditions:\n" );
      fprintf( OutFile,  " The latency   measurements were done with %8d bytes\n", msg_length_for_lat);
      fprintf( OutFile,  " The bandwidth measurements were done with %8d bytes\n", msg_length_for_bw);
      fprintf( OutFile,  " The ring communication was done in both directions on %1d processes\n", size);

      fprintf( OutFile,  " The Ping Pong measurements were done on \n");
      fprintf( OutFile,  "  -  %10.0f pairs of processes for latency benchmarking, and \n", 1.0*number_of_pairs_for_lat);
      fprintf( OutFile,  "  -  %10.0f pairs of processes for bandwidth benchmarking, \n", 1.0*number_of_pairs_for_bw);
      fprintf( OutFile,  " out of %d*(%d-1) = %10.0f possible combinations on %1d processes.\n", size, size, 1.0*size*(size-1), size);
      fprintf( OutFile,  " (1 MB/s = 10**6 byte/sec)\n" );
      fprintf( OutFile,  "\n------------------------------------------------------------------\n");
      fflush( OutFile );
  }
}

void
main_bench_lat_bw(HPCC_Params *params) {
  int myRank, commSize;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

  if (0 == myRank) {
    OutFile = fopen( params->outFname, "a" );
    if (! OutFile) {
      OutFile = stderr;
      fprintf( OutFile, "Cannot open output file.\n" );
    }
  } else OutFile = stderr;

  if (commSize > 1)
  bench_lat_bw_print( &params->MaxPingPongLatency, &params->RandomlyOrderedRingLatency,
                      &params->MinPingPongBandwidth, &params->NaturallyOrderedRingBandwidth,
                      &params->RandomlyOrderedRingBandwidth,
                      &params->MinPingPongLatency, &params->AvgPingPongLatency,
                      &params->MaxPingPongBandwidth, &params->AvgPingPongBandwidth,
                      &params->NaturallyOrderedRingLatency );

  MPI_Bcast( &params->MaxPingPongLatency, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->RandomlyOrderedRingLatency, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->MinPingPongBandwidth, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->NaturallyOrderedRingBandwidth, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->RandomlyOrderedRingBandwidth, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->MinPingPongLatency, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->AvgPingPongLatency, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->MaxPingPongBandwidth, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->AvgPingPongBandwidth, 1, MPI_DOUBLE, 0, comm );
  MPI_Bcast( &params->NaturallyOrderedRingLatency, 1, MPI_DOUBLE, 0, comm );

  fflush( OutFile );
  if (stderr != OutFile) fclose(OutFile);
}
