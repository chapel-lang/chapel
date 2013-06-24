#include "chadoop.h"


int IS_NULL(void* ptr) {
	if (ptr == NULL) {
		return IS_NULL_TRUE;
	} else {
		return IS_NULL_FALSE;
	}
}

void chadoopFree(void* ptr) {
	if (ptr == NULL) {
		return;
	} else {
		free(ptr);
	}
}

void chadoopFreeString(const char* ptr) {
	chadoopFree((void*) ptr);
}

chadoopFileInfo chadoopGetFileInfo(hdfsFS fs, const char* path) {
	chadoopFileInfo res;
	hdfsFileInfo* info = hdfsGetPathInfo(fs, path);
	res.mBlockSize = info->mBlockSize;
//	res.mGroup = info->mGroup;
//	res.mKind = (info->mKind == kObjectKindDirectory) ? "D" : "F";
	res.mLastAccess = info->mLastAccess;
	res.mLastMod = info->mLastMod;
//	res.mName = info->mName;
//	res.mOwner = info->mOwner;
	res.mPermissions = info->mPermissions;
	res.mReplication = info->mReplication;
	res.mSize = info->mSize;
	hdfsFreeFileInfo(info, 1);
	return res;
}

int chadoopGetBlockCount(char*** hostBlocks) {

	int count = 0;
	for (;;) {
		if (hostBlocks[count] != NULL) {
			count++;
		} else {
			break;
		}
	}

	return count;
}

int chadoopGetHostCount(char*** hostBlocks, int block) {

	int count = 0;
	for (;;) {
		if (hostBlocks[block][count] != NULL) {
			count++;
		} else {
			break;
		}
	}

	return count;
}

int printBlockHosts_C(char*** hostBlocks, char* locId){
  int i,j;
  printf("Loc: %s\n ===============================================\n", locId);
  for (i=0; hostBlocks[i]; i++) {
    for (j = 0; hostBlocks[i][j]; j++) {
      printf("%s\t", hostBlocks[i][j]);
    }
    printf("\n");
  }
  printf("=====================================================\n");
  return 0;
}

//const char* chadoopGetHost(char*** hostBlocks, int block, char* prefferedHost, int host) {
//  int i;
//  for (i = 0; hostBlocks[block][i]; i++) {
//    if (hostBlocks[block][i] == prefferedHost)
//      return prefferedHost;
//  }
//  return hostBlocks[block][host];
//}
const char* chadoopGetHost(char*** hostBlocks, int block, int host) {
  return hostBlocks[block][host];
}


const char* chadoopReadFile(hdfsFS fs, hdfsFile file, tSize length) {
  //const char* buffer = malloc(sizeof(char) * length);
  //tSize read;
  //if (buffer == NULL) {
  //  printf("Out of memory\n");
  //  exit(1);
  //}
  //read = hdfsRead(fs, file, (void *)buffer, length);
  //return buffer;
  return chadoopReadFilePositional(fs, file, 0, length);
}

const char* chadoopReadFilePositional(hdfsFS fs, hdfsFile file, tOffset position, tSize length) {

  //const char* buffer = malloc(sizeof(char) * length);
  // WARNING!!!!! MUST be calloc'd and NOT(!!!) malloc'd
  const char* buffer = calloc(length, sizeof(char));
  tSize read;
  if (buffer == NULL) {
    printf("OOM\n");
    exit(1);
  }

  read = hdfsPread(fs, file, position, (void*) buffer, length);
  //assert(read == length);
  return buffer;

}
