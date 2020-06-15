/*
 * Copyright (c) 2013-2014 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _OFI_ABI_H_
#define _OFI_ABI_H_

#include "config.h"

#include <ofi_osd.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
 * ABI version support definitions.
 *
 * CURRENT_ABI:
 * This defines the current ABI version.  The ABI version is separate from
 * the packaging or interface versions.  Whenever a change is
 * added to the interfaces that breaks the ABI, this definition should be
 * updated.  If you don't know if a change breaks the ABI, then you shouldn't
 * be modifying the header files under include/rdma!  :P
 *
 * DEFAULT_SYMVER_PRE:
 * This macro appends an underscore to a function name.  It should be used
 * around any function that is exported from the library as the default call
 * that applications invoke.
 *
 * CURRENT_SYMVER:
 * This macro is placed after a function definition.  It should be used with
 * any function that is exported by the library and was added as part of the
 * current ABI (identified by CURRENT_ABI) version.  It results in the function
 * being exported at the current ABI version.  This is the macro to use when
 * exporting new functions.
 *
 * DEFAULT_SYMVER:
 * This macro is similar to CURRENT_SYMVER, but is used to specify that a
 * function, while the default interface that applications call, was added
 * in a previous version of the ABI.  Any function that was not impacted by
 * an ABI change should use this macro.  This often means converting functions
 * marked as CURRENT_SYMVER to DEFAULT_SYMVER as part of the ABI update.
 *
 * COMPAT_SYMVER:
 * The compatibility symbols are used to mark interfaces which were exported
 * in previous ABI versions, but are no longer the default.  These functions
 * are provided purely for backwards compatibility with existing (already
 * compiled) applications.
 *
 * Example:
 * ABI version 1.0 introduced functions foo() and bar().
 * ABI version 1.1 modified the behavior for function foo().
 * This scenario would result in the following definitions.
 *
 * CURRENT_ABI "MYLIB_1.1"
 *
 * This function is the main entry point for function bar.
 * int DEFAULT_SYMVER_PRE(bar)(void)
 * {
 *    ...
 * }
 * DEFAULT_SYMVER(bar_, bar, "MYLIB_1.0");
 *
 * This function is the main entry point for function foo.
 * int DEFAULT_SYMVER_PRE(foo)(void)
 * {
 *    ...
 * }
 * CURRENT_SYMVER(foo_, foo);
 *
 * This function is the old entry point for function foo, provided for
 * backwards compatibility.
 * int foo_1_0(void)
 * {
 *    ...
 * }
 * COMPAT_SYMVER(foo_1_0, foo, "MYLIB_1.0");
 *
 * By convention, the name of compatibility functions is the exported function
 * name appended with the ABI version that it is compatible with.
 */

#define CURRENT_ABI "FABRIC_1.3"

#if  HAVE_ALIAS_ATTRIBUTE == 1
#define DEFAULT_SYMVER_PRE(a) a##_
#else
#define DEFAULT_SYMVER_PRE(a) a
#endif

/* symbol -> external symbol mappings */
#if HAVE_SYMVER_SUPPORT

#define COMPAT_SYMVER(name, api, ver) \
	asm(".symver " #name "," #api "@" #ver)
#define DEFAULT_SYMVER(name, api, ver) \
	asm(".symver " #name "," #api "@@" #ver)
#define CURRENT_SYMVER(name, api) \
	asm(".symver " #name "," #api "@@" CURRENT_ABI)

#else

#define COMPAT_SYMVER(name, api, ver)

#if HAVE_ALIAS_ATTRIBUTE == 1
#define DEFAULT_SYMVER(name, api, ver) \
	extern typeof (name) api __attribute__((alias(#name)));
#define CURRENT_SYMVER(name, api) \
	extern typeof (name) api __attribute__((alias(#name)));
#else
#define DEFAULT_SYMVER(name, api, ver)
#define CURRENT_SYMVER(name, api)
#endif  /* HAVE_ALIAS_ATTRIBUTE == 1*/

#endif /* HAVE_SYMVER_SUPPORT */


/*
 * The conversion from abi 1.0 requires being able to cast from a newer
 * structure back to the older version.
 */
struct fi_cq_err_entry_1_0 {
	void			*op_context;
	uint64_t		flags;
	size_t			len;
	void			*buf;
	uint64_t		data;
	uint64_t		tag;
	size_t			olen;
	int			err;
	int			prov_errno;
	/* err_data is available until the next time the CQ is read */
	void			*err_data;
};


#ifdef __cplusplus
}
#endif

#endif /* _OFI_ABI_H_ */
