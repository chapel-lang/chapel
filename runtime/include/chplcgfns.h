#ifndef _CHPL_GEN_INTERFACE_H_
#define _CHPL_GEN_INTERFACE_H_

#include <stdlib.h>
#include "chpltypes.h"

/* This header file is for routines that are in the generated code */

/* defined in chpl__header.h: */

extern int32_t maxThreadsPerLocale;
extern uint64_t callStackSize;
extern int32_t chpl_numRealms;

/* defined in ChapelThreads.c: */
void chpl__init_ChapelThreads(int32_t _ln, chpl_string _fn);

/* defined in _config.c: */

const char* chpl_realmType(int32_t r);

/* used for entry point: */
extern void chpl_main(void);

/* used for config vars: */
extern void CreateConfigVarTable(void);

/* used by copy collection: */

/* If we're not in the generated code, define an enum _class_id; */
#ifndef CHPL__CLASS_ID_DEFINED
typedef enum { chpl__cid_bogus } chpl__class_id;
#endif

extern size_t cid2size(chpl__class_id cid);
extern size_t* cid2offsets(chpl__class_id cid);

/* These are defined in _type_structure.c if
   --gen-communicated-structures is true and are used by a
   communication layer to query types of communicated buffers */
extern chplType chpl_getFieldType(int typeNum, int fieldNum);
extern size_t chpl_getFieldOffset(int typeNum, int fieldNum);
extern size_t chpl_getFieldSize(int typeNum);
extern const int chpl_max_fields_per_type;

#endif
