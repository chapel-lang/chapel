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

int main(int argc, char* argv[]) {
  int lgTableSize = 7;
  unsigned long long tableSize;
  unsigned long long NUPDATE;

  if (argc >= 2) {
    lgTableSize = atoi(argv[1]);
  }
  tableSize = 1 << lgTableSize;
  NUPDATE = tableSize*4;
  printf("TableSize  = %llu\n", tableSize);
  printf("NumUpdates = %llu\n", NUPDATE);

  unsigned long long temp = 0x1ULL;
  unsigned long long index = 0;
  int i;
  for (i=0; i<NUPDATE; i++) {
    temp = (temp << 1) ^ (((long long) temp < 0) ? POLY : 0);
    index = temp & (tableSize-1);
    //    index = (temp & ((tableSize-1) << (64 - 7))) >> (64 - 7);
    //    printf("%6llu (%20llu)\n", index, temp);
    printf("%llu %llu\n", index, temp);
  }
}
