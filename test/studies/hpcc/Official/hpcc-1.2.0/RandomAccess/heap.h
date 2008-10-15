
#define HEAP_ROOT   0
#define NOT_A_NODE  (-1)

typedef struct heap_record {
  char*  poolNext;              /* pointer for memory pool */
  int   index;
  int   key;
} Heap_Record, *Heap_Record_Ptr;

extern void HPCC_ra_Heap_Init (int size);
extern void HPCC_ra_Heap_Insert (int index, int key);
extern void HPCC_ra_Heap_ExtractMax (int *index, int *key);
extern void HPCC_ra_Heap_IncrementKey (int index, int key);
extern void HPCC_ra_Heapify (int node);
extern void HPCC_ra_Heap_Free ();
