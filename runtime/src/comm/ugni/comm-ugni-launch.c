//
// Copyright 2012 Cray Inc.  All Rights Reserved.  Unpublished Proprietary
// Information.  This unpublished work is protected to trade secret,
// copyright and other laws.  Except as permitted by contract or express
// written permission of Cray Inc., no part of this work or its content
// may be used, reproduced or disclosed in any form.
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
// Launch assistance for the uGNI communication interface.
//

#define _POSIX_C_SOURCE 200112L  // for setenv(3) in <stdlib.h>

#include <stdlib.h>

#include "chplrt.h"
#include "chpl-comm-launch.h"
#include "error.h"


void chpl_comm_preLaunch(void) {
  if (setenv("HUGETLB_VERBOSE", "0", 1) != 0)
    chpl_error("cannot setenv HUGETLB_VERBOSE=0", 0, 0);
}
