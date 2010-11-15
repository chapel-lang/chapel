//
// Shared code for different comm implementations in comm-*/chpl_cfg_comm.c
//
#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_localeID = -1;
int32_t chpl_numLocales = -1;
int32_t chpl_numPrivateObjects = 0;
static int32_t chpl_capPrivateObjects = 0;
void** chpl_privateObjects;

int chpl_verbose_comm;
int chpl_comm_diagnostics;
int chpl_verbose_mem;

void chpl_newPrivatizedClass(void* v) {
  chpl_numPrivateObjects += 1;
  if (chpl_numPrivateObjects == 1) {
    chpl_capPrivateObjects = 8;
    chpl_privateObjects = chpl_malloc(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_PRIVATE_OBJECTS_ARRAY, 0, "");
  } else {
    if (chpl_numPrivateObjects > chpl_capPrivateObjects) {
      void** tmp;
      chpl_capPrivateObjects *= 2;
      tmp = chpl_malloc(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_PRIVATE_OBJECTS_ARRAY, 0, "");
      memcpy((void*)tmp, (void*)chpl_privateObjects, (chpl_numPrivateObjects-1)*sizeof(void*));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; to do: fix with lock free data structure
    }
  }
  chpl_privateObjects[chpl_numPrivateObjects-1] = v;
}

extern void* chpl_getPrivatizedClass(int32_t i) {
  return chpl_privateObjects[i];
}


//
// The function chpl_comm_get_offset copies data from the memory of
// a locale, to the pointer indicated plus an offset, so it can be used to
// fill a buffer, for example. That cannot be done in Chapel, as 
// it cannot add an offset to a pointer.
//

void  chpl_comm_get_offset(long *addr,long offset, int32_t locale, long* raddr, int32_t size) {
/* DEBUG INFO commented  
   printf("before copying values: ");
   for (int i=0; i < (offset/8)+(size/8);i++) {
	printf(" %ld ",addr[i]);
   }
   printf("\n");
   printf ("amp addr:%ld value:%ld point dat:%ld offset:%ld\n",addr+(offset/8),(*(addr+offset/8)),*addr,offset/8);
   printf ("rdata:%ld size:%d\n",raddr,size);
   fflush(NULL);
   */
   chpl_comm_get(addr+(offset/8), locale, raddr, size, 0, "none");
  /* 
   printf ("new value:%ld\n",(addr+offset));
   printf("copied values: ");
   for (int i=0; i < (offset/8)+(size/8);i++) {
	printf(" %ld ",addr[i]);
   }
   printf("END\n");
   */
}


