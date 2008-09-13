#ifndef _CHPL_GEN_INTERFACE_H_
#define _CHPL_GEN_INTERFACE_H_

#include <stdlib.h>
#include "chpltypes.h"

/* This header file is for routines that are in the generated code */

/* used for entry point: */
extern void chpl_main(void);

/* used for config vars: */
extern void CreateConfigVarTable(void);

/* used by copy collection: */

/* If we're not in the generated code, define an enum _class_id; */
#ifndef CHPL_GEN_CODE
typedef enum { chpl__cid_bogus } chpl__class_id;
#endif

extern size_t cid2size(chpl__class_id cid);
extern size_t* cid2offsets(chpl__class_id cid);

#endif
