//
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-mem-desc.h"
#include "chpltypes.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>


//
// Define the description strings for the memory descriptors.
//
#define CHPL_MEMDESC_DESC(md_name, md_desc)  md_desc
static const char* rt_memDescs[] = {
  CHPL_MD_ALL_MEMDESCS(CHPL_MEMDESC_DESC)
};

const char* chpl_mem_descString(chpl_mem_descInt_t mdi) {
  if (mdi < CHPL_RT_MD_NUM)
    return rt_memDescs[mdi];
  else
    return chpl_mem_descs[mdi-CHPL_RT_MD_NUM];
}
