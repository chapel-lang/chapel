//
// Copyright 2015 Cray Inc.  All Rights Reserved.
//
// U.S. GOVERNMENT RESTRICTED RIGHTS NOTICE
//
// The Computer Software is delivered as "Commercial Computer Software"
// as defined in DFARS 48 CFR 252.227-7014.
//
// All Computer Software and Computer Software Documentation acquired by
// or for the U.S. Government is provided with Restricted Rights.  Use,
// duplication or disclosure by the U.S. Government is subject to the
// restrictions described in FAR 48 CFR 52.227-14 or DFARS 48 CFR
// 252.227-7014, as applicable.
//
// Technical Data acquired by or for the U.S. Government, if any, is
// provided with Limited Rights.  Use, duplication or disclosure by the
// U.S. Government is subject to the restrictions described in FAR 48
// CFR 52.227-14 or DFARS 48 CFR 252.227-7013, as applicable.
//

//
// Memory management declarations for the ugni comm layer.
//

#ifndef _comm_ugni_mem_h_
#define _comm_ugni_mem_h_

#include <stdint.h>

#include "chplrt.h"

#include "chpl-mem.h"


void chpl_comm_mem_reg_add_request(size_t size);

void chpl_comm_mem_reg_init(void);

void chpl_comm_mem_reg_tell(void** space_p, size_t* size_p);

void *chpl_comm_mem_reg_allocMany(size_t number, size_t size,
                                  chpl_mem_descInt_t description,
                                  int32_t lineno, int32_t filename);

void chpl_comm_mem_reg_exit(void);

#endif // _comm_ugni_mem_h_
