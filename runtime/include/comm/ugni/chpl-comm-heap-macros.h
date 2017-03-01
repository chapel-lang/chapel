//
// Copyright 2012-2013 Cray Inc.  All Rights Reserved.  Unpublished
// Proprietary Information.  This unpublished work is protected to trade
// secret, copyright and other laws.  Except as permitted by contract or
// express written permission of Cray Inc., no part of this work or its
// content may be used, reproduced or disclosed in any form.
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
// Heap-related macros for the Chapel uGNI communication layer.
//

#ifndef _comm_heap_macros_h_
#define _comm_heap_macros_h_


void chpl_comm_ugni_help_register_global_var(int i, wide_ptr_t wide);

#define CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide) \
  chpl_comm_ugni_help_register_global_var(i, wide);

#endif

