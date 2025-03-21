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
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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
  GASNETI_IDENT(gasnetc_IdentString_Hwloc, "$GASNetHwloc: lib (API version " _STRINGIFY(HWLOC_API_VERSION) ") $");
#elif GASNETI_HAVE_HWLOC_UTILS
  #include <sys/types.h>
  #include <sys/wait.h>
  #undef HWLOC_API_VERSION
  #define USE_HWLOC_UTILS 1
  typedef const char *gasneti_hwloc_obj_type_t;
  typedef const char *gasneti_hwloc_cpuset_t;
  #define CLOSE_STDIN " </dev/null"
  GASNETI_IDENT(gasnetc_IdentString_Hwloc,
                "$GASNetHwloc: utils (" GASNETI_HWLOC_BIND_PATH "," GASNETI_HWLOC_CALC_PATH ") $");
#else
  #undef USE_HWLOC_LIB
  #undef USE_HWLOC_UTILS
  GASNETI_IDENT(gasnetc_IdentString_Hwloc, "$GASNetHwloc: none $");
#endif

// ------------------------------------------------------------------------------------
//
// File-scope variables
//

static int gasneti_hwloc_is_init = 0;
#if USE_HWLOC_LIB
hwloc_topology_t gasneti_hwloc_topology;
#endif
#if USE_HWLOC_LIB || USE_HWLOC_UTILS
gasneti_hwloc_cpuset_t gasneti_hwloc_cpuset = NULL;
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
static
char *check_suffixed(const char *prefix)
{
  size_t prefix_len = strlen(prefix);
  char *keyname = gasneti_malloc(prefix_len + 3); // 3: '_' + digit + '\0'
  strncpy(keyname, prefix, prefix_len+1);
  keyname[prefix_len] = '_';
  keyname[prefix_len+2] = '\0';

  char *result = NULL;
  for (char c = '0'; c <= '9'; ++c) {
    keyname[prefix_len+1] = c;
    const char* match = gasneti_check_env_prefix(keyname);
    if (match) {
      const char* p = strchr(match, '=');
      result = gasneti_strndup(match, p - match);
      break;
    }
  }

  gasneti_free(keyname);
  return result;
}

// Simple (statically defined and not thread-safe) "set"
// Used to sort and de-dup small integers
typedef unsigned long suff_set_word_t;
static struct { // NOTE: static ensure zero initialization
  unsigned int word_count;
  suff_set_word_t *words;
} gasneti_hwloc_suffix_set;

static void suff_set_insert(unsigned int n, unsigned int div, unsigned int mod)
{
  if (div) n /= div;
  if (mod) n %= mod;

  const size_t word_size = sizeof(suff_set_word_t);
  const unsigned int bits_per_word = 8 * word_size;
  const unsigned int word_idx = n / bits_per_word;
  const unsigned int bit_idx  = n % bits_per_word;

  if (word_idx >= gasneti_hwloc_suffix_set.word_count) {
    unsigned int old_count = gasneti_hwloc_suffix_set.word_count;
    unsigned int new_count = word_idx + 1;
    suff_set_word_t *ptr = gasneti_realloc(gasneti_hwloc_suffix_set.words, word_size * new_count);
    memset(ptr + old_count, 0, word_size * (new_count - old_count));
    gasneti_hwloc_suffix_set.words = ptr;
    gasneti_hwloc_suffix_set.word_count = new_count;
  }

  gasneti_hwloc_suffix_set.words[word_idx] |= ((suff_set_word_t)1) << bit_idx;
}

static char *suff_set_to_string(void)
{
  char *result = NULL;

  const unsigned int bits_per_word = 8 * sizeof(suff_set_word_t);

  unsigned int n = 0;
  const unsigned int word_count = gasneti_hwloc_suffix_set.word_count;
  for (unsigned int word_idx = 0; word_idx < word_count; ++word_idx) {
    suff_set_word_t word = gasneti_hwloc_suffix_set.words[word_idx];
    suff_set_word_t mask = 1;
    for (unsigned int bit_idx = 0; bit_idx < bits_per_word; ++bit_idx, mask <<= 1, ++n) {
      if (word & mask) {
        result = gasneti_sappendf(result, "_%u", n);
      }
    }
  }

  return result;
}

static void suff_set_free(void) {
  gasneti_free(gasneti_hwloc_suffix_set.words);
  gasneti_hwloc_suffix_set.words = NULL;
  gasneti_hwloc_suffix_set.word_count = 0;
}

// Check for the given operator and its operand in a typestring.
// Returns 0 if the operator was not found
// Otherwise:
// + returns the integer operand (which cannot be zero)
// + replace *typestring_p with a string stripped of the operator,
//   operand and optional whitespace before the operator.
// + does NOT free() the prior value of *typestring_p
static int check_op(const char **typestring_p, const char operator, const char* keyname)
{
  const char *typestring = *typestring_p;
  const char *p;
  int result = 0;
  if (NULL != (p = strchr(typestring, operator))) {
    char *endp = NULL;
    result = strtol(p+1, &endp, 0);
    if (result <= 0) { // NOTE: strtol() will return 0 if no digits were seen
       gasneti_fatalerror("Operator %c in value \"%s\" of environment variable "
                          "%s_TYPE must be followed by a positive integer",
                          operator, typestring, keyname);
    }

    while (isspace(*endp)) ++endp; // "be liberal in what you accept"
    if (*endp) {
       gasneti_fatalerror("Value \"%s\" of environment variable %s_TYPE has "
                          "unexpected characters after operator %c and its operand",
                          typestring, keyname, operator);
    }

    // Strip operator+operand and optional preceding whitespace
    gasneti_assert(*p == operator);
    while ((p>typestring) && isspace(*(p-1))) --p;
    size_t keep = p - typestring;
    *typestring_p = gasneti_strndup(typestring, keep);
  }

  return result;
}

// ------------------------------------------------------------------------------------
//
// Initialization and finalization
//

int gasneti_hwloc_init(void) {
  if (gasneti_hwloc_is_init) return 0;

  int cpubind_process;
  const char *envval = gasneti_getenv_withdefault("GASNET_HWLOC_QUERY","thread");
  if (! gasneti_strcasecmp(envval,"thread")) {
    cpubind_process = 0;
  } else if (! gasneti_strcasecmp(envval,"process")) {
    cpubind_process = 1;
  } else {
    gasneti_fatalerror("GASNET_HWLOC_QUERY='%s' is not recognized", envval);
  }

  int result = 0; // assume success

  gasneti_tick_t t1 = GASNETI_TICKS_NOW_IFENABLED(I);
  gasneti_tick_t t2;
  GASNETI_TRACE_PRINTF(I,("gasneti_hwloc_init() {"));
  GASNETI_TRACE_PRINTF(I,("  GASNET_HWLOC_QUERY = '%s'", envval));
#if USE_HWLOC_LIB
  GASNETI_TRACE_PRINTF(I,("  using libhwloc API version 0x%x", HWLOC_API_VERSION));
  int rc = hwloc_topology_init(&gasneti_hwloc_topology);
  if (rc < 0) {
    // failed to initialize hwloc
    GASNETI_TRACE_PRINTF(I,("  failed: hwloc_topology_init() returned %d", rc));
    goto fail_bad_topo;
  }
  // Enable "whole system" mode for uniform counting/naming
  #if HWLOC_API_VERSION >= 0x020100 // since 2.1.0
    (void)hwloc_topology_set_flags(gasneti_hwloc_topology, HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED);
  #else
    (void)hwloc_topology_set_flags(gasneti_hwloc_topology, HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM);
  #endif
  // Enable inclusion (not free) of PCI and O/S devices
  #if HWLOC_API_VERSION >= 0x00020000
    hwloc_topology_set_type_filter(gasneti_hwloc_topology, HWLOC_OBJ_PCI_DEVICE, HWLOC_TYPE_FILTER_KEEP_ALL);
    hwloc_topology_set_type_filter(gasneti_hwloc_topology, HWLOC_OBJ_OS_DEVICE, HWLOC_TYPE_FILTER_KEEP_ALL);
  #else
    hwloc_topology_set_flags(gasneti_hwloc_topology, HWLOC_TOPOLOGY_FLAG_IO_DEVICES);
  #endif
  gasneti_hwloc_cpuset = hwloc_bitmap_alloc();
  if (!gasneti_hwloc_cpuset) {
    GASNETI_TRACE_PRINTF(I,("  failed: hwloc_bitmap_alloc() returned NULL"));
    goto fail_bad_cpuset;
  }
  rc = hwloc_topology_load(gasneti_hwloc_topology);
  if (rc < 0) {
    GASNETI_TRACE_PRINTF(I,("  failed: hwloc_topology_load() returned %d", rc));
    goto fail_bad_cpuset;
  }
  hwloc_cpubind_flags_t cpubind_flags = cpubind_process ? HWLOC_CPUBIND_PROCESS
                                                        : HWLOC_CPUBIND_THREAD;
  rc = hwloc_get_cpubind(gasneti_hwloc_topology, gasneti_hwloc_cpuset, cpubind_flags);
  if (rc < 0) {
    GASNETI_TRACE_PRINTF(I,("  failed: hwloc_get_cpubind() returned %d", rc));
    goto fail_bad_cpuset;
  }

  if (GASNETI_TRACE_ENABLED(I)) {
    size_t len = hwloc_bitmap_snprintf(NULL, 0, gasneti_hwloc_cpuset);
    char *buf = gasneti_malloc(len+1);
    hwloc_bitmap_snprintf(buf, len+1, gasneti_hwloc_cpuset);
    GASNETI_TRACE_PRINTF(I,("  cpuset: %s", buf));
    gasneti_free(buf);
  }

  goto success;

fail_bad_cpuset:
  hwloc_topology_destroy(gasneti_hwloc_topology);
fail_bad_topo:
  result = 1;
  goto done;
#elif USE_HWLOC_UTILS
  GASNETI_TRACE_PRINTF(I,("  using hwloc command-line utilities"));
  // Note: gasneti_hwloc_cpuset_t is "char *" when using utils
  const char *cmd;
  if (cpubind_process) {
    cmd = gasneti_dynsprintf(GASNETI_HWLOC_BIND_PATH " --pid %lu --get" CLOSE_STDIN, (unsigned long)getpid());
  } else {
    cmd = GASNETI_HWLOC_BIND_PATH " --get" CLOSE_STDIN;
  }
  gasneti_hwloc_cpuset = run_hwloc_cmd(cmd, NULL);
  // It is sufficient here to validate that we have a hexadecimal value.
  // `hwloc-calc` will perform stronger validation when we pass this as argument.
  if (!gasneti_hwloc_cpuset ||
      gasneti_hwloc_cpuset[0] != '0' ||
      gasneti_hwloc_cpuset[1] != 'x' ||
      !isxdigit(gasneti_hwloc_cpuset[2])) {
    // failed to query cpu binding from hwloc
    GASNETI_TRACE_PRINTF(I,("  failed to query 'hwloc-bind --get' for cpuset"));
    goto fail_bad_cpuset;
  }

  GASNETI_TRACE_PRINTF(I,("  cpuset: %s", gasneti_hwloc_cpuset));
  goto success;

fail_bad_cpuset:
  gasneti_free((void *)gasneti_hwloc_cpuset);
  result = 1;
  goto done;
#endif

success:
  gasneti_assert(result == 0); // success
  gasneti_hwloc_is_init = 1;
  // fall through

done:
  t2 = GASNETI_TICKS_NOW_IFENABLED(I);
  GASNETI_TRACE_PRINTF(I,("  initialized in %.3fus", gasneti_ticks_to_ns(t2-t1)/1000.0));
  GASNETI_TRACE_PRINTF(I,("}"));
  return result;
}

int gasneti_hwloc_fini(void) {
  if (!gasneti_hwloc_is_init) return 0;

#if USE_HWLOC_LIB
  if (gasneti_hwloc_cpuset) hwloc_bitmap_free(gasneti_hwloc_cpuset);
  hwloc_topology_destroy(gasneti_hwloc_topology);
#elif USE_HWLOC_UTILS
  gasneti_free((void *)gasneti_hwloc_cpuset);
#endif

  gasneti_hwloc_is_init = 0;
  return 0;
}

// ------------------------------------------------------------------------------------
// gasneti_getenv_hwloc_withdefault()
//
// In the steps below "return the value of keyname from the environment" means:
//    Return getenv(keyname) if non-NULL, otherwise return
//    gasneti_getenv_withdefault(keyname, dflt_val).
//    In either case the value is traced as if obtained by the latter.
//    This gives precedence to the local environment (see bug 4303).
//
// 1. Check for env var "[keyname]_TYPE" equal to "None" or prefixed with "Auto" (case insensitive).
//    If "None", return the value of keyname from the environment.
//    If starts with "Auto" then
//      if "enable_auto" is non-zero, return the entire value to caller
//      else continue and the "dflt_type" will be used instead (with a warning)
// 2. Check for suffixed env vars.
//    If none, return the value of keyname from the environment.
// 3. Strip any '/' or '%' suffixes to be applied in later steps
// 4. Check for env var "[keyname]_TYPE" equal to "{J,H,N}Rank" (case insensitive).
//    If YES, return the associated suffixed env var (if any) or the value of keyname from the environment.
// With hwloc support:
//   5. Look for a hwloc object type in env var "[keyname]_TYPE", or dflt_type if none.
//   6. Find the intersection of this proc's cpu binding with options of the given type.
//   7. Return the value of env var "[keyname]_[binding]", if any,
//      otherwise return the value of keyname from the environment
// Without hwloc support:
//   8. If we get this far, warn at most once about lack of hwloc support
//   9. Return the value of keyname from the environment.
//
// Detected hwloc errors result in a warning (at most once per "step")
// and use of the unsuffixed variable.
const char *gasneti_getenv_hwloc_withdefault(const char *keyname, const char *dflt_val, const char *dflt_type, int enable_auto)
{
#if USE_HWLOC_LIB || USE_HWLOC_UTILS
  // Define these early to avoid harmlss goto-bypasses-initialization warnings
  gasneti_hwloc_obj_type_t type = (gasneti_hwloc_obj_type_t)0;
#endif

  const char *suffix = NULL;
  const char *result = NULL;

  // Step 1 - check env var "[keyname]_TYPE" for special cases:
  // + "None" (which disables all additional intelligence)
  // + "Auto" prefix (which will return the typestring to the caller)
  char *typekey = gasneti_sappendf(NULL, "%s_TYPE", keyname);
  const char *typestring = gasneti_getenv_withdefault(typekey, dflt_type);
  const char *orig_typestring = typestring;
  gasneti_free(typekey);
  if (typestring) {
    if (! gasneti_strcasecmp("none", typestring)) {
      // short-cut w/o using hwloc if TYPE is "none"
      goto out_return_unsuffixed;
    }
    if (! gasneti_strncasecmp("auto", typestring, 4) && enable_auto) {
      // inform caller that an "auto" mode has been requested
      return typestring;
    }
  }

  // Step 2 - check for suffixed vars
  char *firstkey = check_suffixed(keyname);
  if (! firstkey) {
    // short-cut w/o using hwloc if there are no suffixed variables
    goto out_return_unsuffixed;
  }

  // Step 3 - strip off any "%" or "/" expressions
  unsigned int div = check_op(&typestring, '/', keyname);
  unsigned int mod = check_op(&typestring, '%', keyname);
  if (div && mod) {
    gasneti_fatalerror("Value \"%s\" of environment variable %s_TYPE includes "
                       "multiple arithmetic operations (at most one is allowed)",
                       orig_typestring, keyname);
  }

  // Step 4 - check env var "[keyname]_TYPE" for "?Rank" (which doesn't need hwloc)
  if (typestring && typestring[0]) {
    gex_Rank_t n = GEX_RANK_INVALID;
    int match = !gasneti_strcasecmp("rank", typestring+1); // Note +1 offset into the string
    if (match) {
      switch (tolower(typestring[0])) {
        case 'j': // 'J'obrank
          n = gasneti_mynode;
          break;
        case 'h': // 'H'ost-relative rank
          n = gasneti_myhost.node_rank;
          break;
        case 'n': // 'N'brhd-relative rank
          n = gasneti_mysupernode.node_rank;
          break;
        default:
          match = 0;
      }
      if (match && (n == GEX_RANK_INVALID)) {
        gasneti_fatalerror("%s value uninitialized in gasneti_getenv_hwloc_withdefault()", typestring);
      }
    }
    if (match) {
      if (div) n /= div;
      if (mod) n %= mod;
      suffix = gasneti_sappendf(NULL, "_%d", n);
      goto try_suffix;
    }
  }
          
#if USE_HWLOC_LIB || USE_HWLOC_UTILS
  // The "real thing" via EITHER libhwloc OR hwloc-{bind,calc}

  // Step 5 - hwloc object type
  // Note non-zero return indicates invalid dflt_type, not a user error
  gasneti_assert_zeroret( get_selector_type(&type, keyname, typestring, dflt_type) );

  // Step 6 - compute intersection between 'cpuset' and object(s) of 'type'
  if (! gasneti_hwloc_is_init) {
    goto out_bad_hwloc;
  }
  #if USE_HWLOC_LIB
  {
    int count = hwloc_get_nbobjs_by_type(gasneti_hwloc_topology, type);
    if (count <= 0) {
      // EITHER there are no objects of the given type
      // OR type occurs at multiple levels (not a type suited to our purposes)
      goto out_bad_intersect;
    }
    for (int i = 0; i < count; ++i) {
      hwloc_obj_t obj = hwloc_get_obj_by_type(gasneti_hwloc_topology, type, i);
      gasneti_assert(obj);
      if (hwloc_bitmap_intersects(gasneti_hwloc_cpuset, obj->cpuset)) {
        suff_set_insert(i, div, mod);
      }
    }
    suffix = suff_set_to_string();
  }
  #else
  { 
    size_t len = 0;
    char *cmd = gasneti_sappendf(NULL, GASNETI_HWLOC_CALC_PATH " --intersect %s %s" CLOSE_STDIN, type, gasneti_hwloc_cpuset);
    char *buf = run_hwloc_cmd(cmd, &len);
    gasneti_free(cmd);
    if (!buf || !buf[0]) {
      // failed to compute the intersection
      gasneti_free(buf);
      goto out_bad_intersect;
    }
    gasneti_assert_uint(strlen(buf) ,==, len);
    for (int i = 0; i < len; ) {
      char c = buf[i];
      if (c == ',') {
        ++i;
      } else if (isdigit(c)) {
        suff_set_insert(atoi(buf+i), div, mod);
        i += strspn(buf+i,"0123456789");
      } else {
        // invalid character (such as in an error message?)
        gasneti_free(buf);
        goto out_bad_intersect;
      }
    }
    suffix = suff_set_to_string();
    gasneti_free(buf);
  }
 #endif
    
  // Step 7 - query the environment with suffix
  goto try_suffix;

out_bad_hwloc:
  {
    static int did_hwloc_warning = 0;
    if (!did_hwloc_warning) {
      gasneti_console_message("WARNING",
          "Failed to query hwloc for cpuset (binding) while processing environment variable '%s'.  "
          "You may set '%s_TYPE=none' to disable checks for suffixed variants of this variable.  "
          "Suppressing additional warnings, if any, for this error with additional variables.",
          keyname, keyname);
      did_hwloc_warning = 1;
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
          typestring, keyname, keyname);
      did_intersect_warning = 1;
    }
    goto out;
  }

#else // !(USE_HWLOC_LIB || USE_HWLOC_UTILS)
  // Fallback when hwloc is unavailable

  // Step 8.  Warn at most once about presence of suffixed keys
  static int did_warn = 0;
  if (!did_warn) {
    gasneti_console_message("WARNING",
        "Ignoring environment variable '%s' (and maybe more) due to lack of hwloc support.  "
        "You may set '%s_TYPE=none' to disable checks for suffixed variants of '%s'",
        firstkey, keyname, keyname);
    did_warn = 1;
  }
  gasneti_free(firstkey);

  // Step 9.  Return the only thing we can
  goto out_return_unsuffixed;
#endif

try_suffix:
  if (suffix && suffix[0]) {
    char *fullkey = gasneti_sappendf(NULL, "%s%s", keyname, suffix);
    GASNETI_TRACE_PRINTF(I,("Query environment variable '%s' for type='%s'",
                            fullkey, orig_typestring));
    result = gasneti_getenv(fullkey);
    if (result) {
      gasnett_envstr_display(fullkey, result, 0);
    }
    gasneti_free(fullkey);
  }
  gasneti_free((void *) suffix); // discarding const qualifier to avoid warning

out:
  if (typestring != orig_typestring) gasneti_free((void*)typestring);
  suff_set_free();
  gasneti_free(firstkey);
  #if USE_HWLOC_UTILS
    // casts below discard const qualifiers to avoid warnings
    gasneti_free((void *)type);
  #endif

  // Return the suffixed variable's value if any, else use unsuffixed
  if (result) return result;

out_return_unsuffixed:
  // Return the unsuffixed "keyname" value from the environment.
  // Here we give precedence to the local environment (bug 4303).
  // TODO: hoist this logic (or similar) for general use
  { char *local_envval = getenv(keyname);
    if (local_envval) {
      gasneti_envstr_display(keyname, local_envval, 0);
      return local_envval;
    }
  }
  return gasneti_getenv_withdefault(keyname, dflt_val);
}

#if USE_HWLOC_LIB && (HWLOC_API_VERSION >= 0x020000)
// Helpers for pu_distance
static int _gasneti_have_numa_latency = 0;
static struct hwloc_distances_s *_gasneti_numa_latency;
static struct hwloc_distances_s *gasneti_get_numa_latencies(void)
{
  if (!_gasneti_have_numa_latency) {
    unsigned int nr = 1;
  #if HWLOC_API_VERSION >= 0x00020100
    int rc = hwloc_distances_get_by_name(gasneti_hwloc_topology, "NUMALatency", &nr, &_gasneti_numa_latency, 0);
  #else
    unsigned long kind = HWLOC_DISTANCES_KIND_FROM_OS | HWLOC_DISTANCES_KIND_MEANS_LATENCY;
    int rc = hwloc_distances_get(gasneti_hwloc_topology, &nr, &_gasneti_numa_latency, kind, 0);
  #endif
    if ((rc < 0) || !nr) _gasneti_numa_latency = NULL;
    _gasneti_have_numa_latency = 1;
  }
  return _gasneti_numa_latency;
}
static void gasneti_release_numa_latencies(void)
{
  if (_gasneti_numa_latency) {
    hwloc_distances_release(gasneti_hwloc_topology, _gasneti_numa_latency);
  }
  _gasneti_numa_latency = NULL;
  _gasneti_have_numa_latency = 0;
}

// Helper for gasneti_hwloc_distances()
//
// The return from this function is an unsigned 32-bit metric representing the
// "distance" from a given PU (named by its physical/OS id) to the given 'obj'.
//
// The 'obj' argument is an hwloc object of "normal" or "io" type.
//
// Current metric:
// + The upper eight bits are the number of levels one must traverse upward in
//   the hwloc hierarchy, starting at the PU, to reach a level which "contains"
//   the given device.  Depending on the node architecture, we've seen instances
//   as near as L3 Cache and as distant as Machine.
// + The lowest eight bits are the minimum of the distances taken from the hwloc
//   "NUMALatency" table between the NUMA nodeset of the PU and that of the
//   device connection point.
//
// TODO:
//   + While hwloc reports the OS-provided PCI "linkspeed" for devices, we do
//     not currently take this information in to account.
//     Use of this information might break ties between devices connected at
//     the same level, but via distinct PCI bus/bridge paths.
//
static uint32_t pu_distance(unsigned int pu_id, hwloc_obj_t obj)
{
  // "io" objects don't have a cpuset or nodeset, so map to a "normal" one above them
  if (hwloc_obj_type_is_io(obj->type)) {
    obj = hwloc_get_non_io_ancestor_obj(gasneti_hwloc_topology, obj);
  }

  // Find depth of the nearest common object relative to the PUs
  unsigned int rel_depth = hwloc_get_type_depth(gasneti_hwloc_topology, HWLOC_OBJ_CORE) - obj->depth;
  hwloc_obj_t common = obj;
  while (common) {
    if (hwloc_bitmap_isset(common->cpuset, pu_id)) break;
    ++rel_depth;
    common = common->parent;
  }
  uint32_t distance = rel_depth << 24;

  hwloc_obj_t pu = hwloc_get_pu_obj_by_os_index(gasneti_hwloc_topology, pu_id);
  if (! hwloc_bitmap_isincluded(pu->nodeset, obj->nodeset)) {
    // PU's NUMA nodeset is not contained in that of the device connection
    // point.  To avoid ties between cases in which the nodesets of devices are
    // not equidistant, this logic adds a NUMA latency component to the
    // distance.
    //
    // Why "isincluded" instead of "intersects"?
    // Skipping the NUMA latency scoring when the PU and device connection point
    // have intersecting nodesets could lead to omitting this metric when, for
    // instance, the only intersection is HBM.  Since we assume transfers
    // to/from DRAM (or caches) are the important metric, such omission would
    // reduce the usefulness of the distance metric.
    //
    // Why use *minimum* latency between NUMA nodes of the PU and device?
    // Alternatives such as maximum, mean or median could "mix" things like HBM
    // in addition to DRAM.  If, for instance, only a subset of sockets have
    // HBM, then their max/mean/median distance could appear greater than the
    // others even if there are no other differences (where in this example we
    // are assuming that high bandwidth comes at the expense of higher latency).
    // TODO: can we take a mean over "fast" memories, perhaps via obj->subtype?
    struct hwloc_distances_s *numa_latencies = gasneti_get_numa_latencies();
    if (numa_latencies) {
      // Distance values in SLIT table are 1-byte (0..255)
      //  0-9     reserved
      //  10      self (diagonal)
      //  11-254  latency metric (off-diagonal)
      //  255     unreachable
      unsigned int min_distance = 256;
      unsigned int i, j;
      hwloc_bitmap_foreach_begin(i, pu->nodeset) {
        hwloc_obj_t nnode1 = hwloc_get_numanode_obj_by_os_index(gasneti_hwloc_topology, i);
        hwloc_bitmap_foreach_begin(j, obj->nodeset) {
          hwloc_obj_t nnode2 = hwloc_get_numanode_obj_by_os_index(gasneti_hwloc_topology, j);
          if (nnode1 == nnode2) continue; // self
          uint64_t fwd, bwd;
          int rc = hwloc_distances_obj_pair_values(numa_latencies, nnode1, nnode2, &fwd, &bwd);
          if (rc || fwd == 255 || bwd == 255) continue; // Invalid or unreachable
          unsigned int dist = (fwd + bwd) / 2; // mean, in case asymmetric
          min_distance = MIN(min_distance, dist);
        } hwloc_bitmap_foreach_end();
      } hwloc_bitmap_foreach_end();
      if (min_distance != 256) {
        distance += min_distance;
      }
    }
  }

  GASNETI_TRACE_PRINTF(I,("    PU L#%u is in cpuset of %s L#%d, %d levels away, NUMALatency %d",
                           pu->logical_index, hwloc_obj_type_string(common->type), common->logical_index,
                           rel_depth, distance & 0xff));

  gasneti_assert( distance ); // logic elsewhere assumes non-zero
  return distance;
}

// Helper for gasneti_hwloc_distances()
//
// The return from this function is the mean over pu_distance()
// for every PU in the process cpuset.
// NOTE: making *one* pu_distance() call for the "process is INSIDE cpuset"
// case (see trace logic below) did not prove to be an important optimization.
static uint32_t proc_distance(hwloc_obj_t dev)
{
  hwloc_obj_t obj = hwloc_get_non_io_ancestor_obj(gasneti_hwloc_topology, dev);

  if (GASNETI_TRACE_ENABLED(I)) {
    GASNETI_TRACE_PRINTF(I,("  %s L#%d, connected at %s L#%d",
                            hwloc_obj_type_string(dev->type), dev->logical_index,
                            hwloc_obj_type_string(obj->type), obj->logical_index));

    if (hwloc_bitmap_isincluded(gasneti_hwloc_cpuset, obj->cpuset)) {
      GASNETI_TRACE_PRINTF(I,("  process is INSIDE cpuset of %s L#%d",
                              hwloc_obj_type_string(obj->type), obj->logical_index));
    } else {
      GASNETI_TRACE_PRINTF(I,("  process %s cpuset of %s L#%d",
                              (hwloc_bitmap_intersects(gasneti_hwloc_cpuset, obj->cpuset) ? "INTERSECTS" : "is OUTSIDE"),
                              hwloc_obj_type_string(obj->type), obj->logical_index));
    }
  }

  // Take the mean over all PUs in proc cpuset
  uint32_t distance = 0;
  uint64_t sum = 0;
  unsigned int pu_id;
  hwloc_bitmap_foreach_begin(pu_id, gasneti_hwloc_cpuset) {
    sum += pu_distance(pu_id, obj);
  } hwloc_bitmap_foreach_end();
  distance = sum / hwloc_bitmap_weight(gasneti_hwloc_cpuset);

  GASNETI_TRACE_PRINTF(I,("  distance 0x%08x", (unsigned int)distance));
  return distance;
}
#endif //  USE_HWLOC_LIB && (HWLOC_API_VERSION >= 0x020000)

int gasneti_hwloc_distances(int count, uint32_t *distances, const char **names, unsigned int flags)
{
#if USE_HWLOC_LIB && (HWLOC_API_VERSION >= 0x020000)
  if (! gasneti_hwloc_is_init) return -1;

  uint32_t *raw = (flags & GASNETI_HWLOC_DISTANCES_NORMALIZE)
                      ? gasneti_malloc(count * sizeof(uint32_t)) // temporary
                      : distances;                               // in-place

  int retval = 0;
  for (int i = 0; i < count; ++i) {
    const char *name = names[i];
    GASNETI_TRACE_PRINTF(I,("gasneti_hwloc_distances(%s): {", name));
    // Try as a PCI ID string, and then as an OS device name
    hwloc_obj_t obj = hwloc_get_pcidev_by_busidstring(gasneti_hwloc_topology, name);
    if (!obj) {
      while (NULL != (obj = hwloc_get_next_osdev(gasneti_hwloc_topology, obj))) {
        if (strcmp(name,obj->name)) continue;
        break;
      }
    }
    if (obj) {
      raw[i] = proc_distance(obj);
      ++retval;
    } else {
      raw[i] = GASNETI_HWLOC_DISTANCE_UNKNOWN;
      GASNETI_TRACE_PRINTF(I,("  not found"));
    }
    GASNETI_TRACE_PRINTF(I,("}"));
  }

  gasneti_release_numa_latencies();

  // Optionally normalize raw distance values to yield 0-based ones
  if (flags & GASNETI_HWLOC_DISTANCES_NORMALIZE) {
    for (int i = 0; i < count; ++i) {
      distances[i] = GASNETI_HWLOC_DISTANCE_UNKNOWN;
    }
    int remain = retval; // number of devices hwloc could find
    for (int norm_dist = 0; remain; ++norm_dist) {
      // Find lowest raw distance among unnormalized devices
      uint32_t best = GASNETI_HWLOC_DISTANCE_UNKNOWN;
      for (int i = 0; i < count; ++i) {
        if (distances[i] == GASNETI_HWLOC_DISTANCE_UNKNOWN) {
          best = MIN(best, raw[i]);
        }
      }
      if (best == GASNETI_HWLOC_DISTANCE_UNKNOWN) break;
      // Score the devices(s) with the current best distance
      for (int i = 0; i < count; ++i) {
        if (raw[i] == best) {
          distances[i] = norm_dist;
          remain--;
        }
      }
      gasneti_assert_always(remain >= 0);  // else infinite loop!
    }
    gasneti_free(raw);
  }

  gasneti_assert_int(retval ,>=, 0);
  return retval;
#else
  GASNETI_TRACE_PRINTF(I,("gasneti_hwloc_distances(0x%x) failure: libhwloc 2.0+ required", flags));
  return -1;
#endif
}

// ------------------------------------------------------------------------------------
//
// Automatic device selection
//

// Array of unique device names, where indices are used in place of names
// to identify devices.
// NOTE: anticipated number of devices per host isn't large enough to warrant
// use of a more-complex data structure.
static const char **dev_names = NULL;
static int dev_name_count = 0;

// Adds a name to the array if not already present.
// Returns the array index for the name, regardless of whether new or not.
static int dev_name_to_idx(const char *name) {
  int i;
  for (i = 0; i < dev_name_count; ++i) {
    if (! strcmp(name, dev_names[i])) return i;
  }
  gasneti_assert(i == dev_name_count);
  dev_names = gasneti_realloc(dev_names, ++dev_name_count * sizeof(const char *));
  dev_names[i] = gasneti_strdup(name);
  return i;
}

struct dev_table_row {
  int row_len;
  int assigned;
  struct dev_table_tuple {
    int dev_idx;
    int use;
  } *tuples;
};

// Write out file w/ this proc's list on "nearest" of devices.
// File format is a series of newline-terminated lines:
// + First line is device count as base-10 integer (as "%d")
// + Remainder are device (string) as "%s"
//   This allows device names to contain anything but \0 and \n.
static void auto_dev_write_inputs(FILE *fp, int count, const char **names, uint32_t *distances)
{
  int row_len = 0;
  for (int i = 0; i < count; ++i) {
    if (! distances[i]) ++row_len;
  }
  if (fprintf(fp, "%d\n", row_len) < 0) goto error;
  for (int i = 0; i < count; ++i) {
    if (distances[i]) continue; // skip - not near
    if (fprintf(fp, "%s\n", names[i]) < 0) goto error;
  }
  fflush(fp);
  return;

error:
  gasneti_fatalerror("bad file write in automatic device selection");
}

// Read file generated by auto_dev_write_inputs()
static void auto_dev_read_inputs(FILE *fp, struct dev_table_row *row)
{
  int row_len;
  if (fscanf(fp, " %d ", &row_len) < 0) goto error;
  row->row_len = row_len;
  row->tuples = gasneti_calloc(row_len, sizeof(struct dev_table_tuple));
  struct dev_table_tuple *p = &row->tuples[0];
  size_t buflen = 0;
  char *buf = NULL;
  for (int i = 0; i < row_len; ++i, ++p) {
    size_t line_len = gasneti_getline(&buf, &buflen, fp);
    if (line_len < 1) goto error;
    // replace \n with \0
    gasneti_assert(buf[line_len - 1] == '\n');
    buf[line_len-1] = 0;
    // convert to index of name in table of unique names
    p->dev_idx = dev_name_to_idx(buf);
  }
  gasneti_free(buf);
  return;

error:
  gasneti_fatalerror("bad file read in automatic device selection");
}

// lead process writes one row of the solution
static void auto_dev_write_ouput(FILE *fp, struct dev_table_row *row)
{
  const int row_len = row->row_len;
  struct dev_table_tuple *p = &row->tuples[0];
  char *soln_string = gasneti_malloc((row_len + 1) * sizeof(char));
  for (int i = 0; i < row_len; ++i, ++p) {
    soln_string[i] = p->use ? '1' : '0';
  }
  soln_string[row_len] = '\0';
  if (fprintf(fp, "%s", soln_string) < 0) goto error;
  // NOTE: skip fflush(fp) since caller will fclose(fp)
  gasneti_free(soln_string);
  return;

error:
  gasneti_fatalerror("bad file write in automatic device selection");
}

// A "set-contains" operation (helper for auto_dev_best_row)
//
// Return 1 if the given dev_idx appears in the passed row
static int auto_dev_in_row(const struct dev_table_row *row, int dev_idx) {
  for (int i = 0; i < row->row_len; ++i) {
    if (row->tuples[i].dev_idx == dev_idx) return 1;
  }
  return 0;
}

// Compute one element of the solution (helper for auto_dev_solve)
//
// Returns the host-local rank (row index) of the "best" row for the given
// device index.  Here, "best" is defined as the row with the lowest `row_idx`
// among those which include `dev_idx` among their `tuples[].dev_idx`, but
// excluding those which are already marked as `assigned`.
// Will return impossible `nproc` if no rows satisfy the constraints above.
static int auto_dev_best_row(int nprocs, const struct dev_table_row *dev_table, int dev_idx) {
  int best_len = dev_name_count + 1; // impossibly large row_len
  int best_row = nprocs;             // impossibly large host-local rank (row index)
  for (int i = 0; i < nprocs; ++i) {
    const struct dev_table_row *row = &dev_table[i];
    if (row->assigned) continue;   // this proc has already been assigned s device
    if (! auto_dev_in_row(row, dev_idx)) continue; // device is not near this proc
    if (row->row_len < best_len) { // MIN(row_len) == most constrained proc
      best_len = row->row_len;
      best_row = i;
    }
  }
  return best_row;
}

// compute the solution for automatic device selection
// For every row in dev_table, sets `use` in up to  `max_result` tuples.
// TODO: support max_results != 1
static void auto_dev_solve(int max_results, gex_Rank_t nprocs, struct dev_table_row *dev_table)
{
  gasneti_assert_int(max_results ,==, 1);

  // This algorithm is device-centric.
  // The devices take turns, round-robin, picking one process which will use
  // that device (if any remain).  Each pick is a process which has identified
  // the corresponding device as near, but does not yet have an assigned device.
  // Among such processes, each pick will select one with the fewest devices
  // identified as near.

  const int num_devs = dev_name_count;
  int remain = nprocs; // number of unassigned procs
  for (int curr_dev = 0; remain; curr_dev = ((curr_dev + 1) % num_devs)) {
    int pick = auto_dev_best_row(nprocs, dev_table, curr_dev);
    if (pick == nprocs) continue; // no unassigned procs near this device
    struct dev_table_row *row = &dev_table[pick];
    row->assigned = 1;
    for (int i = 0; i < dev_table[pick].row_len; ++i) {
      if (row->tuples[i].dev_idx == curr_dev) {
        row->tuples[i].use = 1;
        break;
      }
    }
    remain -= 1;
  }
}

// TODO: error returns in place of some fatal errors?
// TODO: mode that favors use of all NICs even if one or more is not
// "nearest" to any process?
int gasneti_hwloc_auto_select(
                int max_results,
                int count,
                int *use,              // OUT
                const char **devices,  // IN
                int verbose,
                gasneti_bootstrapBarrierfn_t barrierfn,
                gasneti_bootstrapBroadcastfn_t hostbcastfn)
{
  uint32_t *distances = gasneti_malloc(count * sizeof(uint32_t));
  if (count == 1) {
    // Don't even need to initialize hwloc when there is exactly one dev locally.
    distances[0] = 0;
  } else {
    int rc = gasneti_hwloc_distances(count, distances, devices, GASNETI_HWLOC_DISTANCES_NORMALIZE);
    if (rc <= 0) {
      gasneti_free(distances);
      return rc;
    }
  }

  // TODO: when gasneti_myhost.node_count == 1, could skip the mkstemp() and
  // corresponding unlink() and possibly more.  However, every process must
  // still enter each of the bootstrap collectives.

  gex_Rank_t leader = gasneti_myhost.nodes[0];
  gex_Rank_t nprocs = gasneti_myhost.node_count;
  gex_Rank_t myproc = gasneti_myhost.node_rank;
  int i_am_lead = !myproc;
  FILE **files = NULL; // non-NULL only for lead process
  FILE *my_file = NULL;

  // Lead process generates a unique temporary file and one for each non-zero
  // host-local-rank.  Host-scoped broadcast of the unique portion of the first
  // file is sufficient to name all of the others.
  // Post-bcast fopen() calls validate the assumption of a single-valued tmpdir.
  const char *tmpdir = gasneti_tmpdir();
  if (!tmpdir && !tmpdir[0]) {
    gasneti_fatalerror("automatic device selection requires valid $GASNET_TMPDIR, $TMPDIR or /tmp");
  }
  char *prefix = gasneti_sappendf(NULL, "%s/GEXtmpXXXXXX", tmpdir);
  if (i_am_lead) {
    int fd = mkstemp(prefix);
    if (fd < 0) {
      gasneti_fatalerror("mkstemp() failed in automatic device selection");
    }
    close(fd);
    files = gasneti_malloc(nprocs * sizeof(FILE *));
    const int flags = O_RDWR | O_CREAT | O_EXCL;
    const int mode = 0600;
    for (gex_Rank_t r = 1; r < nprocs; ++r) {
      const char *filename = gasneti_dynsprintf("%s%d", prefix, r);
      int tmp = open(filename, flags, mode);
      if (tmp < 0) {
        gasneti_fatalerror("open(O_CREAT) failed in automatic device selection");
      }
      FILE *file = fdopen(tmp, "r+");
      if (!file) {
        gasneti_fatalerror("fdopen() failed in automatic device selection");
      }
      files[r] = file;
    }
  }
  {
    char *unique = prefix + strlen(prefix) - 6;
    hostbcastfn(unique, 6, unique, leader);
  }

  // Non-lead procs open and unlink their respective temporary files, where this
  // order ensures that the only possible file leaks are of *empty* files.
  // Next, the (device,distance) pairs are written to the temporary files.
  // A barrier after the writes lets the lead know when it can read the files.
  // This is effectively the first half of a host-scoped GatherV.
  if (! i_am_lead) {
    const char *filename = gasneti_dynsprintf("%s%d", prefix, myproc);
    my_file = fopen(filename, "r+");
    if (!my_file) {
        gasneti_fatalerror("open() failed in automatic device selection");
    }
    (void)unlink(filename);
    auto_dev_write_inputs(my_file, count, devices, distances);
  }
  barrierfn();

  // Lead process now:
  // + Unlinks the original file (from mkstemp()), since there is no further
  //   need for uniqueness (with all other files written and unlinked).
  // + Reads the per-proc temporary files to builds a table of all
  //   (device,distance) tuples the temporary files.  As part of this step, each
  //   device name is converted to a zero-based integer index into a table of
  //   all of the device names (effectively the second half of a host-scoped
  //   GatherV)
  // + Some "magic happens" to perform device assignment
  // + Writes assignments for non-leads are to their respective temp files
  //   (effectively a host-scoped Scatter)
  struct dev_table_row *dev_table = NULL; // used only in leads
  if (i_am_lead) {
    (void)unlink(prefix); // no longer needed to ensure uniqueness
    dev_table = gasneti_calloc(nprocs, sizeof(struct dev_table_row));
    // Fill own row from local variables, skipping devices "UNKNOWN" to hwloc
    int row_len = 0;
    for (int i = 0; i < count; ++i) {
      if (! distances[i]) ++row_len;
    }
    dev_table[0].row_len = row_len;
    dev_table[0].tuples = gasneti_calloc(row_len, sizeof(struct dev_table_tuple));
    for (int i = 0; i < row_len; ++i) {
      if (distances[i]) continue; // skip - not near
      dev_table[0].tuples[i].dev_idx = dev_name_to_idx(devices[i]);
    }
    // Read other rows from the temporary files
    for (gex_Rank_t rank = 1; rank < nprocs; ++rank) {
      auto_dev_read_inputs(files[rank], &dev_table[rank]);
    }

    // perform device assignment
    auto_dev_solve(max_results, nprocs, dev_table);

    // Write assignments for non-leads are to their respective temp files
    for (gex_Rank_t rank = 1; rank < nprocs; ++rank) {
      auto_dev_write_ouput(files[rank], &dev_table[rank]);
      fclose(files[rank]); // is also a fflush()
    }
  }
  barrierfn();

  // Leads construct their solution from dev_table
  // Non-leads read their solution from temp files
  for (int i = 0, j = 0; i < count; ++i) {
    if (! distances[i]) { // nearest
      if (i_am_lead) {
        use[i] = dev_table[0].tuples[j++].use;
      } else {
        // TODO: auto_dev_read_ouput() abstraction?
        int c = fgetc(my_file);
        if_pf (c == EOF) {
          gasneti_fatalerror("bad file read in automatic device selection");
        }
        use[i] = (c == '1') ? 1 : 0;
      }
    } else if (distances[i] == GASNETI_HWLOC_DISTANCE_UNKNOWN) {
      use[i] = -1; // hwloc did not locate
    } else if (distances[i]) {
      use[i] = 0; // not nearest
    }
  }

  // cleanups:
  gasneti_free(prefix);
  if (i_am_lead) {
    gasneti_free(files);
    for (gex_Rank_t rank = 0; rank < nprocs; ++rank) {
      gasneti_free(dev_table[rank].tuples);
    }
    gasneti_free(dev_table);
    for (int i = 0; i < dev_name_count; ++i) {
      gasneti_free((void*)dev_names[i]);
    }
    gasneti_free(dev_names);
  } else {
    fclose(my_file);
  }

  if (verbose) {
    // TODO: a "display name" argument would give the conduit control over the
    // output, which may be important for ofi-conduit where devices[] is
    // expected to contain PCI bus addresses.
    gasneti_console_message("INFO", "Automatic device selection summary (%d devices):", count);
    for (int i = 0; i < count; ++i) {
      const char *status;
      if (use[i] < 0) {
        status = "not found";
      } else if (use[i] > 0) {
        gasneti_assert_uint(distances[i] ,==, 0);
        status = "distance=0 [selected]";
      } else {
        status = gasneti_dynsprintf("distance=%u", (unsigned int)distances[i]);
      }
      gasneti_console_message("INFO", "    device '%s' %s", devices[i], status);
    }
  }

  gasneti_free(distances);

  return 1;
}
