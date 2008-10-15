/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */
/*
  hpcc.c
*/

#include <hpcc.h>
#include <ctype.h>

int
main(int argc, char *argv[]) {
  int myRank, commSize;
  char *outFname;
  FILE *outputFile;
  HPCC_Params params;
  time_t currentTime;

  MPI_Init( &argc, &argv );

  if (HPCC_Init( &params ))
    goto hpcc_end;

  MPI_Comm_size( MPI_COMM_WORLD, &commSize );
  MPI_Comm_rank( MPI_COMM_WORLD, &myRank );

  outFname = params.outFname;

  /* -------------------------------------------------- */
  /*                       PTRANS                       */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of PTRANS section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunPTRANS) PTRANS( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of PTRANS section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                        HPL                         */
  /* -------------------------------------------------- */

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of HPL section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunHPL) HPL_main( argc, argv, &params.HPLrdata, &params.Failure );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of HPL section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                    StarDGEMM                       */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of StarDGEMM section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunStarDGEMM) HPCC_StarDGEMM( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of StarDGEMM section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                    SingleDGEMM                     */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of SingleDGEMM section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunSingleDGEMM) HPCC_SingleDGEMM( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of SingleDGEMM section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                    StarSTREAM                      */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of StarSTREAM section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunStarStream) HPCC_StarStream( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of StarSTREAM section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                    SingleSTREAM                    */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of SingleSTREAM section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunSingleStream) HPCC_SingleStream( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of SingleSTREAM section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                 MPI RandomAccess                   */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of MPIRandomAccess section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunMPIRandomAccess) HPCC_MPIRandomAccess( &params );

  time( &currentTime );
  BEGIN_IO(myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of MPIRandomAccess section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                  StarRandomAccess                  */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of StarRandomAccess section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunStarRandomAccess) HPCC_StarRandomAccess( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of StarRandomAccess section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                 SingleRandomAccess                 */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of SingleRandomAccess section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunSingleRandomAccess) HPCC_SingleRandomAccess( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of SingleRandomAccess section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                       MPIFFT                       */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of MPIFFT section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunMPIFFT) HPCC_MPIFFT( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of MPIFFT section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                      StarFFT                       */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of StarFFT section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunStarFFT) HPCC_StarFFT( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of StarFFT section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                      SingleFFT                     */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of SingleFFT section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunSingleFFT) HPCC_SingleFFT( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of SingleFFT section.\n" );
  END_IO( myRank, outputFile );

  /* -------------------------------------------------- */
  /*                  Latency/Bandwidth                 */
  /* -------------------------------------------------- */

  MPI_Barrier( MPI_COMM_WORLD );

  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile, "Begin of LatencyBandwidth section.\n" );
  END_IO( myRank, outputFile );

  if (params.RunLatencyBandwidth) main_bench_lat_bw( &params );

  time( &currentTime );
  BEGIN_IO( myRank, outFname, outputFile);
  fprintf( outputFile,"Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "End of LatencyBandwidth section.\n" );
  END_IO( myRank, outputFile );

  hpcc_end:

  HPCC_Finalize( &params );

  MPI_Finalize();
  return 0;
}
