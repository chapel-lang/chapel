
#define RA_TIME_BOUND 1
#undef RA_TIME_BOUND

/* time bound in seconds */
#define TIME_BOUND 60


/* _RA_SAMPLE_FACTOR determines the fraction of the total number
 * of updates used (in time_bound.c) to empirically derive an
 * upper bound for the  number of updates executed by the benchmark.
 * This upper bound must be such that the total execution time of the
 * benchmark does not exceed a specified time bound.
 * _RA_SAMPLE_FACTOR may need to be adjusted for each architecture
 * since the dafault number of updates depends on the total
 * memory size.
 */
/* 1% of total number of updates */
#define _RA_SAMPLE_FACTOR 100

extern void
HPCC_Power2NodesTime(u64Int logTableSize,
       u64Int TableSize,
       u64Int LocalTableSize,
       u64Int MinLocalTableSize,
       u64Int GlobalStartMyProc,
       u64Int Top,
       int logNumProcs,
       int NumProcs,
       int Remainder,
       int MyProc,
       MPI_Datatype INT64_DT,
       double timeBound,
       u64Int* numIter,
       MPI_Status *finish_statuses,
       MPI_Request *finish_req);


extern void HPCC_AnyNodesTime(u64Int logTableSize,
    u64Int TableSize,
    u64Int LocalTableSize,
    u64Int MinLocalTableSize,
    u64Int GlobalStartMyProc,
    u64Int Top,
    int logNumProcs,
    int NumProcs,
    int Remainder,
    int MyProc,
    MPI_Datatype INT64_DT,
    double timeBound,
    u64Int* numIter,
    MPI_Status *finish_statuses,
    MPI_Request *finish_req);
