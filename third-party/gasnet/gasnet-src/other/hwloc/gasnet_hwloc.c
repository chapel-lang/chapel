/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/hwloc/gasnet_hwloc.c $
 * Description: GASNet conduit-independent hwloc utilities
 * Copyright 2021, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_hwloc_internal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if GASNETI_HAVE_HWLOC_LIB
  #include "hwloc.h"
  #ifndef HWLOC_API_VERSION
    #error hwloc.h did not define HWLOC_API_VERSION
  #endif
  #define USE_HWLOC_LIB 1
  #undef USE_HWLOC_UTILS
  typedef hwloc_obj_type_t gasneti_hwloc_obj_type_t;
  typedef hwloc_cpuset_t   gasneti_hwloc_cpuset_t;
  #if HWLOC_API_VERSION < 0x010100 // "cpuset" names used prior to 1.1.0
    #define hwloc_bitmap_alloc hwloc_cpuset_alloc
    #define hwloc_bitmap_intersects hwloc_cpuset_intersects
    #define hwloc_bitmap_free hwloc_cpuset_free
  #endif
#elif GASNETI_HAVE_HWLOC_UTILS
  #include <sys/types.h>
  #include <sys/wait.h>
  #undef HWLOC_API_VERSION
  #define USE_HWLOC_UTILS 1
  typedef const char *gasneti_hwloc_obj_type_t;
  typedef const char *gasneti_hwloc_cpuset_t;
  #define CLOSE_STDIN " </dev/null"
#else
  #undef USE_HWLOC_LIB
  #undef USE_HWLOC_UTILS
#endif

// ------------------------------------------------------------------------------------
#if USE_HWLOC_LIB || USE_HWLOC_UTILS
//
// Utilities specific to use of hwloc
// These hide lib-vs-utils and, when necessary, differences across API versions
//

// Convert string to gasneti_hwloc_obj_type_t
// Returns 0 on success, negative on error
static int string_to_obj_type(const char *string, gasneti_hwloc_obj_type_t *result)
{
  #if USE_HWLOC_UTILS
    *result = gasneti_strdup(string);
    return 0;
  #elif HWLOC_API_VERSION >= 0x020000 // since 2.0.0
    return hwloc_type_sscanf(string, result, NULL, 0);
  #elif HWLOC_API_VERSION >= 0x010900 // since 1.9.0
    return hwloc_obj_type_sscanf(string, result, NULL, NULL, 0);
  #else
    *result = hwloc_obj_type_of_string(string);
    return ((int)(*result) < 0) ? -1 : 0;
  #endif
}

// Convert 'typestring' to gasneti_hwloc_obj_type_t.
// If invalid, falls back to 'dflt_type, with a warning.
// Returns 0 on success, negative on error.
// Result is in `*result'.
static int get_selector_type(
                gasneti_hwloc_obj_type_t *result,
                const char *keyname,
                const char *typestring,
                const char *dflt_type)
{
  int rc = string_to_obj_type(typestring, result);
  if (rc < 0) {
    gasneti_console_message("WARNING",
                            "%s_TYPE = '%s' is invalid.  Using default '%s' instead.",
                            keyname, typestring, dflt_type);
    rc = string_to_obj_type(dflt_type, result);
    gasneti_assert_int(rc ,==, 0);
  }

  // Currently non-zero only if dflt_type is invalid
  return rc;
}
#endif // USE_HWLOC_LIB || USE_HWLOC_UTILS

#if USE_HWLOC_UTILS
// Wraps popen(cmd) and gasneti_getline() with appropriate error handling.
// Returns the first line of output (stripped of trailing newline, if any) or NULL on error.
// Caller should gasneti_free() the result.
// If 'len_p' is non-NULL, receives line length on success (not written on failure).
static char *run_hwloc_cmd(const char *cmd, size_t *len_p)
{
  char *output = NULL;

  FILE *stream = popen(cmd, "r");
  if (!stream) {
    // failed popen()
    // probably a failed fork(), pipe(), or malloc() (maybe should be fatal?)
    return NULL;
  }

  size_t n = 0;
  ssize_t line_len = gasneti_getline(&output, &n, stream);
  int status = pclose(stream);
  if (!WIFEXITED(status) || WEXITSTATUS(status) || (line_len < 0)) {
    // one or both of pclose() or getline() says that the command failed
    gasneti_free(output);
    return NULL;
  }

  // strip trailing newline, if any
  if (line_len && (output[line_len-1] == '\n')) {
    output[line_len-1] = '\0';
    line_len -= 1;
  }

  if (len_p) *len_p = line_len;
  return output;
}
#endif // USE_HWLOC_UTILS

// Look for any "_N"-suffixed env vars.
// Returns gasneti_malloc'ed string with first matched variable name, if any.
// TODO: could be more efficient if given an interface to iterate over the environment keys.
static
char *check_suffixed(const char *keyname)
{
  size_t keylen = strlen(keyname);
  size_t fulllen = keylen + 5; // 4 = "_xx\0"
  char *fullkey = gasneti_malloc(fulllen);
  strcpy(fullkey, keyname);
  strcat(fullkey, "_");
  for (int i = 0; i < 64; ++i) { // TODO: any realistic situation in which this is not enough?
    snprintf(fullkey+(keylen+1), fulllen-(keylen+1), "%d", i);
    gasneti_assert_uint(strlen(fullkey) ,<, fulllen);
    if (gasneti_getenv(fullkey)) {
      return fullkey;
    }
  }
  gasneti_free(fullkey);
  return NULL;
}

// ------------------------------------------------------------------------------------
// gasneti_getenv_hwloc_withdefault()
//
// 1. Check for suffixed env vars.
//    If none, return result of gasneti_getenv_withdefault(keyname, dflt_val).
// 2. Check for env var "[keyname]_TYPE" equal to "None" (case insensitive).
//    If YES, return result of gasneti_getenv_withdefault(keyname, dflt_val).
// With hwloc support:
//   3. Look for a hwloc object type in env var "[keyname]_TYPE", or dflt_type if none.
//   4. Find the intersection of this proc's cpu binding with options of the given type.
//   5. Return the value of env var "[keyname]_[binding]", if any,
//      otherwise return the result of gasneti_getenv_withdefault(keyname, dflt_val).
// Without hwloc support:
//   3. If we get this far, warn at most once about lack ofhwloc support
//   4. Return the result of gasneti_getenv_withdefault(keyname, dflt_val).
//
// Detected hwloc errors result in a warning (at most once per "step")
// and use of the unsuffixed variable.
char *gasneti_getenv_hwloc_withdefault(const char *keyname, const char *dflt_val, const char *dflt_type)
{
  char *result = NULL;

  // Step 1 - check for suffixed vars
  char *firstkey = check_suffixed(keyname);
  if (! firstkey) {
    // short-cut w/o using hwloc if there are no suffixed variables
    return gasneti_getenv_withdefault(keyname, dflt_val);
  }

  // Step 2 - check env var "[keyname]_TYPE" for "None" (which disables all additional intelligence)
  char *typekey = gasneti_sappendf(NULL, "%s_TYPE", keyname);
  const char *typestring = gasneti_getenv_withdefault(typekey, dflt_type);
  gasneti_free(typekey);
  if (typestring) {
    const char none[] = "none";
    int match = 1;
    for (int i = 0; i < sizeof(none); ++i) { // includes final '\0'
      if (tolower(typestring[i]) != none[i]) {
        match = 0;
        break;
      }
    }
    if (match) {
      // short-cut w/o using hwloc if TYPE is "none"
      gasneti_free(firstkey);
      return gasneti_getenv_withdefault(keyname, dflt_val);
    }
  }
          
#if USE_HWLOC_LIB || USE_HWLOC_UTILS
  // The "real thing" via EITHER libhwloc OR hwloc-{bind,calc}
  char *suffix = NULL;
  gasneti_hwloc_obj_type_t type = (gasneti_hwloc_obj_type_t)0;
  gasneti_hwloc_cpuset_t cpuset = NULL;

  // Step 3 - hwloc object type
  // Note non-zero return indicates invalid dflt_type, not a user error
  gasneti_assert_zeroret( get_selector_type(&type, keyname, typestring, dflt_type) );

  // Step 4a - query the current proc's cpu binding
  #if USE_HWLOC_LIB
    int topo_is_init = 0;
    hwloc_topology_t topology;
    if (hwloc_topology_init(&topology) < 0) {
      // failed to initialize hwloc
      goto out_bad_cpuset;
    }
    topo_is_init = 1;
    // Enable "whole system" mode for uniform counting/naming
    #if HWLOC_API_VERSION >= 0x020100 // since 2.1.0
      (void)hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED);
    #else
      (void)hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM);
    #endif
    cpuset = hwloc_bitmap_alloc();
    if (!cpuset ||
        (hwloc_topology_load(topology) < 0) ||
        (hwloc_get_cpubind(topology, cpuset, HWLOC_CPUBIND_PROCESS) < 0 )) {
      // failed to query cpu binding from hwloc
      goto out_bad_cpuset;
    }
  #else
    // Note: gasneti_hwloc_cpuset_t is "char *" when using utils
    cpuset = run_hwloc_cmd(GASNETI_HWLOC_BIND_PATH " --get" CLOSE_STDIN, NULL);
    // It is sufficient here to validate that we have a hexadecimal value.
    // `hwloc-calc` will perform stronger validation when we pass this as argument.
    if (!cpuset || cpuset[0] != '0' || cpuset[1] != 'x' || !isxdigit(cpuset[2])) {
      // failed to query cpu binding from hwloc
      goto out_bad_cpuset;
    }
  #endif

  // Step 4b - compute intersection between 'cpuset' and object(s) of 'type'
  #if USE_HWLOC_LIB
  {
    int count = hwloc_get_nbobjs_by_type(topology, type);
    if (count <= 0) {
      // EITHER there are no objects of the given type
      // OR type occurs at multiple levels (not a type suited to our purposes)
      goto out_bad_intersect;
    }
    for (int i = 0; i < count; ++i) {
      hwloc_obj_t obj = hwloc_get_obj_by_type(topology, type, i);
      gasneti_assert(obj);
      if (hwloc_bitmap_intersects(cpuset, obj->cpuset)) {
        suffix = gasneti_sappendf(suffix, "_%d", i);
      }
    }
  }
  #else
  { 
    size_t len = 0;
    char *cmd = gasneti_sappendf(NULL, GASNETI_HWLOC_CALC_PATH " --intersect %s %s" CLOSE_STDIN, type, cpuset);
    char *buf = run_hwloc_cmd(cmd, &len);
    gasneti_free(cmd);
    if (!buf || !buf[0]) {
      // failed to compute the intersection
      gasneti_free(buf);
      goto out_bad_intersect;
    }
    gasneti_assert_uint(strlen(buf) ,==, len);
    // In one pass: copy (to prepend '_'), validate and translate ',' to '_'.
    // Note: `--sep X` option to set the delimiter was not available in older hwloc-calc.
    suffix = gasneti_malloc(len + 2); // +2 = leading '_' and trailing '\0'
    suffix[0] = '_';
    for (int i = 0; i < len; ++i) {
      char c = buf[i];
      if (c == ',') {
        suffix[i+1] = '_';
      } else if (isdigit(c)) {
        suffix[i+1] = c;
      } else {
        // invalid character (such as in an error message?)
        gasneti_free(buf);
        goto out_bad_intersect;
      }
    }
    suffix[len+1] = '\0';
    gasneti_free(buf);
  }
 #endif
    
  // Step 5 - query the environment with suffix
  if (suffix && suffix[0]) {
    char *fullkey = gasneti_sappendf(NULL, "%s%s", keyname, suffix);
    result = gasneti_getenv(fullkey);
    if (result) {
      gasnett_envstr_display(fullkey, result, 0);
    }
    gasneti_free(fullkey);
  }
  gasneti_free(suffix);

out:
  gasneti_free(firstkey);
  #if USE_HWLOC_LIB
    if (cpuset) hwloc_bitmap_free(cpuset);
    if (topo_is_init) hwloc_topology_destroy(topology);
  #elif USE_HWLOC_UTILS
    // casts below discard const qualifiers to avoid warnings
    gasneti_free((void *)type);
    gasneti_free((void *)cpuset);
  #endif

  // Return the suffixed variable's value if any, else use unsuffixed
  return result ? result : gasneti_getenv_withdefault(keyname, dflt_val);

out_bad_cpuset:
  {
    static int did_cpuset_warning = 0;
    if (!did_cpuset_warning) {
      gasneti_console_message("WARNING",
          "Failed to query hwloc for cpuset (binding) while processing environment variable '%s'.  "
          "You may set '%s_TYPE=none' to disable checks for suffixed variants of this variable.  "
          "Suppressing additional warnings, if any, for this error with additional variables.",
          keyname, keyname);
      did_cpuset_warning = 1;
    }
    goto out;
  }

out_bad_intersect:
  {
    static int did_intersect_warning = 0;
    if (!did_intersect_warning) {
      gasneti_console_message("WARNING",
          "Failed to query hwloc for objects of type '%s' while processing environment variable '%s'.  "
          "You may set '%s_TYPE=none' to disable checks for suffixed variants of this variable.  "
          "Suppressing additional warnings, if any, for this error with additional variables.",
          typestring, keyname, keyname), keyname;
      did_intersect_warning = 1;
    }
    goto out;
  }

#else // !(USE_HWLOC_LIB || USE_HWLOC_UTILS)
  // Fallback when hwloc is unavailable

  // Step 3.  Warn at most once about presence of suffixed keys
  static int did_warn = 0;
  if (!did_warn) {
    gasneti_console_message("WARNING",
        "Ignoring environment variable '%s' (and maybe more) due to lack of hwloc support.  "
        "You may set '%s_TYPE=none' to disable checks for suffixed variants of '%s'",
        firstkey, keyname, keyname);
    did_warn = 1;
  }
  gasneti_free(firstkey);

  // Step 4.  Return the only thing we can
  return gasneti_getenv_withdefault(keyname, dflt_val);
#endif
}
