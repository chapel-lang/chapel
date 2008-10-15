
#define HPCC_NULL_PTR ((char *)0)

typedef struct Pool_s {        /* used to minimize the use of malloc */
  char *head;                  /* pointer to the first element of the pool */
  char *tail;                  /* pointer to the last element of the pool */
  int   numObjs;               /* number of objects to malloc */
  int   objSize;               /* size of objects in bytes */
  char *poolBase;              /* pointer to block of memory allocated for pool */
} POOL;

extern POOL* HPCC_PoolInit(int numObjs, int objSize);
extern char *HPCC_PoolGetObj(POOL *poolPtr);
extern void HPCC_PoolReturnObj(POOL *poolPtr, void *objPtr);
extern void HPCC_PoolFree(POOL *poolPtr);

