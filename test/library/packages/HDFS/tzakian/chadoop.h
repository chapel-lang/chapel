//#include "../../hadoop-1.1.2/src/c++/libhdfs/hdfs.h"
//#include "hdfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <hdfs.h>
#include <assert.h>

#define IS_NULL_TRUE 1
#define IS_NULL_FALSE 0

//typedef struct __hdfsString {
//	int length;
//	const char* buffer;
//} _hdfsString, *hdfsString;

typedef struct _chadoopFileInfo {
//    const char* mKind;   /* file or directory */
//    const char *mName;         /* the name of the file */
    tTime mLastMod;      /* the last modification time for the file in seconds */
    tOffset mSize;       /* the size of the file in bytes */
    short mReplication;    /* the count of replicas */
    tOffset mBlockSize;  /* the block size for the file */
//    const char *mOwner;        /* the owner of the file */
//    const char *mGroup;        /* the group associated with the file */
    short mPermissions;  /* the permissions associated with the file */
    tTime mLastAccess;    /* the last access time for the file in seconds */
} chadoopFileInfo;

int IS_NULL(void* ptr);

int printBlockHosts_C(char*** hostBlocks, char* locId);

void chadoopFree(void* ptr);
void chadoopFreeString(const char* str);

chadoopFileInfo chadoopGetFileInfo(hdfsFS fs, const char* path);

int chadoopGetBlockCount(char*** hostBlocks);
int chadoopGetHostCount(char*** hostBlocks, int block);
const char* chadoopGetHost(char*** hostBlocks, int block, int host);

const char* chadoopReadFile(hdfsFS fs, hdfsFile file, tSize length);
const char* chadoopReadFilePositional(hdfsFS fs, hdfsFile file, tOffset position, tSize length);

