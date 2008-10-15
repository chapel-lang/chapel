
typedef struct update_s {
  char  *poolNext;              /* pointer for memory pool */
  u64Int value;
  struct update_s *forward;
} Update_T, *Update_Ptr;

#define NULL_UPDATE_PTR ((Update_Ptr) NULL)

typedef struct pe_bucket_s {
  int numUpdates;
  Update_Ptr updateList;
} Bucket_T, *Bucket_Ptr;

#define NULL_BUCKET_PTR ((Bucket_Ptr) NULL)

extern Bucket_Ptr HPCC_InitBuckets(int numPEs, int maxNumUpdates);
extern void HPCC_FreeBuckets(Bucket_Ptr buckets, int numPEs);
extern void HPCC_InsertUpdate(u64Int ran, int pe,  Bucket_Ptr buckets);
extern int HPCC_GetUpdates(Bucket_Ptr buckets,  u64Int *buffer, int bufferSize, int *peUpdates);
