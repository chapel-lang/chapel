/* 
 * This program was constructed from the HPC-provided single
 * processor version of Random Access so that it would run
 * outside of the HPC suite driver routines.  
 * 
 * Below are defines from various include files that are needed
 * by Random Access.
 *
 * For now, memory size is set to 1000.0.
 *
 * For now, timing is not done and GUPs is not computed.  This
 * program performs the updates to the Table and checks to see
 * if they are correct.
 *
 * last revised 9/18/2006 by marybeth.
 */

#define POLY 0x0000000000000007ULL
#define PERIOD 1317624576693539401LL
#define NUPDATE (4 * TableSize)
#define FSTR64 "%lld"
#define XMALLOC(t,s) ((t*)malloc(sizeof(t)*(s)))


unsigned long long HPCC_starts(long long n) {
  int i, j;
  unsigned long long m2[64];
  unsigned long long temp, ran;

  if (n == 256) printf("n = 256\n");
  while (n < 0) n += PERIOD;
  while (n > PERIOD) n -= PERIOD;
  if (n == 0) return 0x1;

  temp = 0x1;
  for (i=0; i<64; i++) {
    m2[i] = temp;
    temp = (temp << 1) ^ ((long long) temp < 0 ? POLY : 0);
    temp = (temp << 1) ^ ((long long) temp < 0 ? POLY : 0);
  }

  for (i=62; i>=0; i--)
    if ((n >> i) & 1)
      break;

  ran = 0x2;
  while (i > 0) {
    temp = 0;
    for (j=0; j<64; j++)
      if ((ran >> j) & 1)
        temp ^= m2[j];
    ran = temp;
    i -= 1;
    if ((n >> i) & 1)
      ran = (ran << 1) ^ ((long long) ran < 0 ? POLY : 0);
  }

  return ran;
}

static void
RandomAccessUpdate(unsigned long long TableSize, unsigned long long *Table) {
  long long i;
  unsigned long long ran[128];              /* Current random numbers */
  int j;

  /* Initialize main table */
  for (i=0; i<TableSize; i++) Table[i] = i;

  /* Perform updates to main table.  The scalar equivalent is:
   *
   *     unsigned long long ran;
   *     ran = 1;
   *     for (i=0; i<NUPDATE; i++) {
   *       ran = (ran << 1) ^ (((long long) ran < 0) ? POLY : 0);
   *       table[ran & (TableSize-1)] ^= ran;
   *     }
   */
  for (j=0; j<128; j++)
    ran[j] = HPCC_starts ((NUPDATE/128) * j);

  for (i=0; i<NUPDATE/128; i++) {
    for (j=0; j<128; j++) {
      ran[j] = (ran[j] << 1) ^ ((long long) ran[j] < 0 ? POLY : 0);
      Table[ran[j] & (TableSize-1)] ^= ran[j];
    }
  }
}
int main() {
  long long i;
  unsigned long long temp;
  double cputime;               /* CPU time to update table */
  double realtime;              /* Real time to update table */
  double totalMem;
  unsigned long long *Table;
  unsigned long long logTableSize, TableSize;

  double *GUPs;
  int *failure;

  /* calculate local memory per node for the update table */
  totalMem = 1000.0;
  totalMem /= sizeof(unsigned long long);

  /* calculate the size of update array (must be a power of 2) */
  for (totalMem *= 0.5, logTableSize = 0, TableSize = 1;
       totalMem >= 1.0;
       totalMem *= 0.5, logTableSize++, TableSize <<= 1)
    ; /* EMPTY */

  Table = XMALLOC( unsigned long long, TableSize );
  if (! Table) {
      printf("Failed to allocate memory for the update table (" FSTR64 ").\n", TableSize);
    return 1;
  }

  /* Print parameters for run */
  printf("Main table size   = 2^" FSTR64 " = " FSTR64 " words\n", logTableSize,TableSize);
  printf("Number of updates = " FSTR64 "\n", NUPDATE);


  RandomAccessUpdate( TableSize, Table );


  /* make sure no division by zero */
/*  *GUPs = (realtime > 0.0 ? 1.0 / realtime : -1.0); */
/*  *GUPs *= 1e-9*NUPDATE; */
  /* Print timing results */

  /* Verification of results (in serial or "safe" mode; optional) */
  temp = 0x1ULL;
  for (i=0; i<NUPDATE; i++) {
    temp = (temp << 1) ^ (((long long) temp < 0) ? POLY : 0);
    Table[temp & (TableSize-1)] ^= temp;
  }

  temp = 0;
  for (i=0; i<TableSize; i++)
    if (Table[i] != i)
      temp++;

  printf( "Found " FSTR64 " errors in " FSTR64 " locations (%s).\n",
        temp, TableSize, (temp <= 0.01*TableSize) ? "passed" : "failed");
}
