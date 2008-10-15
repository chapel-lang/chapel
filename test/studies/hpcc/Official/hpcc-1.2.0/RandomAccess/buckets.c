/* buckets.c
 *
 * Each process (PE) has a set of buckets, one for each possible
 * destination PE. Each set of buckets is implementated as an
 * array of objects, one for each destination PE, where each object
 * keeps the number of updates currently in the bucket and a pointer
 * to a list of updates.
 * The motivation for using lists (instead of fixed size buckets)
 * is to keep the memory requirements low as the number of processes
 * increase. To avoid the overheads of allocating memory dynamically,
 * a pool of memory is previously allocated and objetcs are
 * allocated/returned from/to this pool (see pool.c for details).
 *
 * An auxiliary data structure keeps the local buckets ordered
 * according to the number of updates of each bucket (see heap.c).
 *
 */


#include <hpcc.h>
#include "RandomAccess.h"
#include "buckets.h"
#include "heap.h"
#include "pool.h"

/* memory pool for updates */
static POOL *Update_Pool;

Bucket_Ptr HPCC_InitBuckets(int numPEs, int maxNumUpdates)
{
  Bucket_Ptr Buckets;
  int i;

  Buckets = (Bucket_Ptr) malloc (numPEs * sizeof(Bucket_T));
  for (i=0; i<numPEs; i++) {
    Buckets[i].numUpdates = 0;
    Buckets[i].updateList = NULL_UPDATE_PTR;
  }

  /* initialize memory pool for updates */
  Update_Pool = HPCC_PoolInit (maxNumUpdates, sizeof(Update_T));

  /* initialize heap of PE's with pending updates */
  HPCC_ra_Heap_Init(numPEs);

  return(Buckets);
}


void HPCC_InsertUpdate(u64Int ran, int pe, Bucket_Ptr Buckets)
{

  Update_Ptr update;
  Bucket_Ptr bucket;
  int numUpdates;

  bucket = Buckets + pe; /* bucket = &(Buckets[pe]); */
  update = (Update_T*) HPCC_PoolGetObj(Update_Pool);
  update->value = ran;
  update->forward = bucket->updateList;
  bucket->updateList = update;
  bucket->numUpdates++;

  numUpdates = bucket->numUpdates;
  if (numUpdates == 1) {  /* this is the first update for this PE since last send */
    HPCC_ra_Heap_Insert (pe, numUpdates);
  }
  else { /* PE already in heap, just increment number of updates */
    HPCC_ra_Heap_IncrementKey(pe, numUpdates);
  }

}



int HPCC_GetUpdates(Bucket_Ptr Buckets, u64Int *bufferPtr, int bufferSize, int *peUpdates)
{

  int pe;
  Bucket_Ptr bucket;
  Update_Ptr update, tmp;
  u64Int *buffer;

  HPCC_ra_Heap_ExtractMax (&pe, peUpdates);
  bucket = Buckets + pe; /* bucket = &(Buckets[pe]); */

  /* copy updates to buffer */
  update = bucket->updateList;
  buffer = bufferPtr;
  while (update != NULL_UPDATE_PTR) {
    *buffer = (u64Int)(update->value);
    buffer ++;
    tmp = update;
    update = update->forward;
    HPCC_PoolReturnObj(Update_Pool, tmp);
  }

  *peUpdates = bucket->numUpdates;
  bucket->numUpdates = 0;
  bucket->updateList = NULL_UPDATE_PTR;

  return(pe);

}


void HPCC_FreeBuckets (Bucket_Ptr Buckets, int numPEs)
{

  Update_Ptr ptr1, ptr2;
  int i;

  HPCC_ra_Heap_Free();

  for (i = 0; i < numPEs; i ++) {
    ptr1 = Buckets[i].updateList;
    while (ptr1 != NULL_UPDATE_PTR) {
      ptr2 = ptr1;
      ptr1 = ptr1->forward;
      HPCC_PoolReturnObj(Update_Pool, ptr2);
    }
  }

  HPCC_PoolFree(Update_Pool);
  free(Update_Pool);
  free (Buckets);

}
