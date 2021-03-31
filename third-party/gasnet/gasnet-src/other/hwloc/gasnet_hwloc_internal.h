/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/hwloc/gasnet_hwloc_internal.h $
 * Description: GASNet conduit-independent hwloc utilities internal header
 * Copyright 2021, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_HWLOC_INTERNAL_H
#define _GASNET_HWLOC_INTERNAL_H

// For a given keyname:
// 1. Look for a hwloc object type in env var "[kename]_TYPE", or dflt_type if none.
// 2. Find the current procs binding(s) for the given type
// 3. Return the value of env var "[keyname]_[binding]", or of "[keyname]" if none
extern char *gasneti_getenv_hwloc_withdefault(
                const char *keyname,
                const char *dflt_val,
                const char *dflt_type);

#endif // _GASNET_HWLOC_INTERNAL_H
