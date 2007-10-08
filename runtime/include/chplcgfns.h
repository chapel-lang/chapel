#ifndef _CHPL_GEN_INTERFACE_H_
#define _CHPL_GEN_INTERFACE_H_

#include <stdlib.h>
#include "chpltypes.h"

/* This header file is for routines that are in the generated code */

/* used for entry point: */
extern void _chpl_main(void);

/* used for config vars: */
extern void CreateConfigVarTable(void);

/* used by copy collection: */

/* If we're not in the generated code, define an enum _class_id; */
#ifndef _CHPL_GEN_CODE
typedef enum { _e_bogus } _class_id;
#endif

extern size_t cid2size(_class_id cid);
extern size_t* cid2offsets(_class_id cid);

#endif
