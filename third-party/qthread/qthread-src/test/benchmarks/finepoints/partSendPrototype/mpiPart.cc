#include <map>
#include <qthread/qthread.h>
#include <unistd.h>
#include <cstdarg>
//#include <omp.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <stdio.h>

int idx;

//#define PRINTF(args,...)  printf( args, ##__VA_ARGS__ )
#define PRINTF(args,...)

static inline int getMyIndex()
{
  return idx;
}


static inline void createComm( MPI_Comm comm, int n, const int ranks[], MPI_Comm* newComm )
{
  int rc;
  MPI_Group   groupWorld, newGroup;
  MPI_Comm_group( comm, &groupWorld);
  MPI_Group_incl( groupWorld, n, ranks, &newGroup );
  rc = MPI_Comm_create( comm, newGroup, newComm );
  assert( rc == MPI_SUCCESS );
  int rank,size;
  int worldRank;
  rc = MPI_Comm_rank( comm, &worldRank );
  rc = MPI_Comm_rank( *newComm, &rank );
  rc = MPI_Comm_size( *newComm, &size );
  PRINTF("%s():%d: worldRank=%d rank=%d size=%d\n",__func__,__LINE__,worldRank,rank,size);
}

struct Info {
  MPI_Win win;
  const void* buf;
  int count;
  int chunkSize;
  MPI_Datatype datatype;
  int tag;
  int myRank;
  int numThreads;
  int numSent;
  MPI_Request request;
  MPI_Comm comm;
  int isSend;
};
#ifdef MPICH

std::map<MPI_Request*,Info*> _infoMap;
static inline void setRequest( MPI_Request* req, Info* info ) {
  _infoMap[req] = info;
}

static inline Info* getInfo( MPI_Request* req ) {
  return _infoMap[req];
}

static inline void clearReq( MPI_Request* req ) {
  _infoMap.erase( req );
}

#else
static inline void setRequest( MPI_Request* req, Info* info ) {
  *req = (struct ompi_request_t*)info;
}
static inline Info* getInfo( MPI_Request* req ) {
  return (Info*)*req;
}
static inline void clearReq( MPI_Request* req ) {
}
#endif

int MPI_Partitioned_Send_create(const void *buf, int count, MPI_Datatype datatype, int numThreads,
                                int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int rc;
  int dataTypeSize;
  struct Info* info = (struct Info*) malloc( sizeof (struct Info ) );

  rc = MPI_Comm_rank( MPI_COMM_WORLD, &info->myRank );
  MPI_Type_size(datatype,&dataTypeSize);
  info->chunkSize = count/numThreads;

  PRINTF("%s():%d: rank=%d numThreads=%d count=%d dataTypeSize=%d chunkSize=%d enter \n",
         __func__,__LINE__,info->myRank,numThreads,count,dataTypeSize,info->chunkSize);

  setRequest( request, info );
  info->buf = buf;
  info->count = count;
  info->datatype = datatype;
  info->tag = tag;
  info->numThreads = numThreads;
  info->numSent = 0;
  info->isSend = 1;

  int ranks[2] = { info->myRank, dest };
  createComm( comm, 2, ranks, &info->comm );

  rc = MPI_Win_create( NULL, 0, dataTypeSize, MPI_INFO_NULL, info->comm, &info->win );
  assert( rc == MPI_SUCCESS );

  assert( 0 ==  (dataTypeSize * count) % numThreads );

  PRINTF("%s():%d: rank=%d leave\n",__func__,__LINE__,info->myRank);
  return MPI_SUCCESS;
}

int MPI_Partitioned_free( MPI_Request* request )
{
  struct Info* info = getInfo(request);
  clearReq(request);
  PRINTF("%s():%d: rank=%d leave\n",__func__,__LINE__,info->myRank);
  int rc = MPI_Win_free( &info->win );
  PRINTF("%s():%d: rank=%d leave\n",__func__,__LINE__,info->myRank);
  assert( rc == MPI_SUCCESS );
  free( info );
  return MPI_SUCCESS;
}

int MPI_Partitioned_Recv_create(void *buf, int count, MPI_Datatype datatype, int src,
                                int tag, MPI_Comm comm, MPI_Request *request)
{
  int rc;
  int dataTypeSize;
  struct Info* info = (struct Info*) malloc( sizeof (struct Info ) );

  MPI_Type_size(datatype,&dataTypeSize);
  rc = MPI_Comm_rank( MPI_COMM_WORLD, &info->myRank );

  PRINTF("%s():%d: rank=%d count=%d dataTypeSize=%d enter \n",__func__,__LINE__,info->myRank,count,dataTypeSize);

  setRequest( request, info );
  info->buf = buf;
  info->count = count;
  info->datatype = datatype;
  info->tag = tag;
  info->isSend = 0;

  int ranks[2] = { src, info->myRank };
  createComm( comm, 2, ranks, &info->comm );

  rc = MPI_Win_create( (void*) buf, count* dataTypeSize, dataTypeSize, MPI_INFO_NULL,
                       info->comm, &info->win );
  assert( rc == MPI_SUCCESS );


  PRINTF("%s():%d: rank=%d leave\n",__func__,__LINE__,info->myRank);
  return MPI_SUCCESS;
}

aligned_t nslock;

int MPI_Partitioned_Add_to_buffer( MPI_Request* request, const void* send_buf,
                                   int count, MPI_Datatype datatype )
{
  int numSent;
  int rc;
  struct Info* info = getInfo(request);
  int index = getMyIndex();
  MPI_Aint displacement = index * info->chunkSize;

  PRINTF("%s():%d:%d: rank=%d tid=%d count=%d displacement=%lu\n",
         __func__,__LINE__,getpid(),info->myRank, index, count, displacement);


  rc = MPI_Put( send_buf, count, datatype, 1, displacement, count, datatype, info->win );
  assert( rc == MPI_SUCCESS );
  qthread_lock(&nslock);
  numSent = ++info->numSent;
  qthread_unlock(&nslock);
  PRINTF("%s():%d: rank=%d tid=%d numSent=%d numThreads=%d\n",__func__,__LINE__,info->myRank, index, info->numSent, info->numThreads);

  if ( info->numThreads == numSent ) {

    PRINTF("%s():%d: rank=%d done\n",__func__,__LINE__,info->myRank);
    rc = MPI_Win_unlock_all( info->win );
    assert( rc == MPI_SUCCESS );

    rc = MPI_Send( NULL, 0, MPI_CHAR, 1, info->tag, info->comm );
    assert( rc == MPI_SUCCESS );
  }

  PRINTF("%s():%d: rank=%d return\n",__func__,__LINE__,info->myRank);
  return MPI_SUCCESS;
}

int MPI_Start_part( MPI_Request* req )
{
  int rc;
  struct Info* info = getInfo(req);

  PRINTF("%s():%d: rank=%d %d %s\n",__func__,__LINE__,info->myRank, getMyIndex(), info->isSend ? "Send":"Recv");

  if ( info->isSend ) {
    rc = MPI_Win_lock_all( 0, info->win );
    assert( rc == MPI_SUCCESS );
    info->numSent = 0;
  }

  return MPI_SUCCESS;
}

int MPI_Wait_part( MPI_Request* req, MPI_Status* status )
{
  int rc;
  struct Info* info = getInfo(req);

  PRINTF("%s():%d: rank=%d %d enter %s\n",__func__,__LINE__,info->myRank, getMyIndex(), info->isSend ? "Send":"Recv" );

  if ( ! info->isSend ) {
    rc = MPI_Recv( NULL, 0, MPI_CHAR, 0, info->tag, info->comm, MPI_STATUS_IGNORE );
    assert( rc == MPI_SUCCESS );
  }
  PRINTF("%s():%d: rank=%d %d leave %s\n",__func__,__LINE__,info->myRank, getMyIndex(), info->isSend ? "Send":"Recv" );

  return MPI_SUCCESS;
}
