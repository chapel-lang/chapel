/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/hwloc/gasnet_hwloc_internal.h $
 * Description: GASNet conduit-independent hwloc utilities internal header
 * Copyright 2021, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_HWLOC_INTERNAL_H
#define _GASNET_HWLOC_INTERNAL_H

// Initialization of GASNet's hwloc subsystem.
extern int gasneti_hwloc_init(void);

// Finalization of GASNet's hwloc subsystem.
extern int gasneti_hwloc_fini(void);

// For a given keyname:
// 1. Look for a hwloc object type in env var "[kename]_TYPE", or dflt_type if none.
// 2. Find the current procs binding(s) for the given type
// 3. Return the value of env var "[keyname]_[binding]", or of "[keyname]" if none
extern const char *gasneti_getenv_hwloc_withdefault(
                const char *keyname,
                const char *dflt_val,
                const char *dflt_type,
                int enable_auto);

// Score devices by distance from the calling processes.
//
// This function takes two arrays ("distances" and "names") of length "count".
// The "names" array is an input, containing strings which can be O/S device
// names known to hwloc, or PCI addresses of the form xxxx:yy:zz.t or yy:zz.t.
//
// The distances[] array is an output, which is populated on success with a
// uint32_t values expressing the mean distance between the PUs in the cpuset
// queried in `gasneti_hwloc_init()` process and the corresponding device.  If
// hwloc cannot map a given names[] element to a device, the corresponding
// element of distances[] will be GASNETI_HWLOC_DISTANCE_UNKNOWN.  Excluding
// such entries, there are two cases for the "distance" elements:
//   IFF GASNETI_HWLOC_DISTANCES_NORMALIZE is set in flags
//     Nearest device(s) will be assigned a distance of 0.
//     Next-nearest device(s) will be assigned a distance of 1, etc.
//   Otherwise, raw uint32_t distances are produced.  In this case
//   larger unsigned values correspond to further topological distance,
//   though the relationship may be non-linear.
//
// Returns a negative value if hwloc is not available, is too old (API 2.0+ is
// required), has not been properly initialized, or any other condition which
// would prevent determining the distance of valid names[] elements.
// In such cases, the content of distances[] will be undefined.
// On success, returns the number of names[] elements successfully processed.
// Together these mean that (retval > 0) is sufficient to determine that there
// is at least one "meaningful" value in distances[].
extern int gasneti_hwloc_distances(
                int count,
                uint32_t *distances, // OUT
                const char **names,  // IN
                unsigned int flags);
#define GASNETI_HWLOC_DISTANCE_UNKNOWN (~(uint32_t)0)
enum {
  GASNETI_HWLOC_DISTANCES_NORMALIZE = (1 <<  0),
};

// Selects as many as 'max_results' devices for the conduit to use, with
// load-balance over processes on the same host.
//
// This function takes two arrays ("use" and "names") of length "count".
// The "names" array is an input, containing strings which can be O/S device
// names known to hwloc, or PCI addresses of the form xxxx:yy:zz.t or yy:zz.t.
//
// On success (positive return value) the elements of `use[]` are set as follows
// to indicate if the caller should use the device indicated by the
// corresponding element in `names[]`:
//   + negative if hwloc could not locate the device.
//   + positive if the device was selected for use.
//   + zero if the corresponding device was located, but was not selected.
//
// The `max_results` argument indicates the number of devices which the caller
// desires to use.  This is an upper bound on the number of positive values in
// `use[]`.
// NOTE: ** The current implementation only supports `max_results==1` **
//
// The `verbose` argument is a boolean to enable console messages with info
// on the auto device selection, eg as determined by the caller from the value
// of `GASNET_IBV_PORTS_VERBOSE` or equivalent for other conduits.
//
// The `barrierfn` is a (non-NULL) pointer to a function to perform a
// global bootstrap barrier.
// The `hostbcastfn` is a (non-NULL) pointer to a function to perform a
// host-scoped bootstrap broadcast.
//
// Returns a negative value if hwloc is not available, is too old (API 2.0+ is
// required), has not been properly initialized, or any other condition which
// would prevent determining the topological distance of valid names[] elements.
// In such cases, the content of use[] will be undefined.
// On success, returns the number of non-zeros in use[].  This will never
// exceed MIN('max_results, 'count'), and may be less if some of the `names[]`
// could not be identified by hwloc.
// Together these mean that (retval > 0) is sufficient to determine that there
// is at least one positive value in use[].
//
// TBD: may want the "auto*" string from GASNET_*_TYPE as an argument, under the
// assumption it will be used to control behavior(s) of this function.
// Alternatively, the conduit might do all the work parsing the optional part.
// In which case, a `flags` argument might be needed to request behaviors.
extern int gasneti_hwloc_auto_select(
                int max_results,
                int count,
                int *use,            // OUT
                const char **names,  // IN
                int verbose,
                gasneti_bootstrapBarrierfn_t barrierfn,
                gasneti_bootstrapBroadcastfn_t hostbcastfn);

#endif // _GASNET_HWLOC_INTERNAL_H
