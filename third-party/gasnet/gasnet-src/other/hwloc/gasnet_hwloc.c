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
// gasneti_getenv_hwloc_withdefault()
//
// In the steps below "return the value of keyname from the environment" means:
//    Return getenv(keyname) if non-NULL, otherwise return
//    gasneti_getenv_withdefault(keyname, dflt_val).
//    In either case the value is traced as if obtained by the latter.
//    This gives precedence to the local environment (see bug 4303).
//
// 1. Check for suffixed env vars.
//    If none, return the value of keyname from the environment.
// 2. Check for env var "[keyname]_TYPE" equal to "None" (case insensitive).
//    If YES, return the value of keyname from the environment.
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
char *gasneti_getenv_hwloc_withdefault(const char *keyname, const char *dflt_val, const char *dflt_type)
{
#if USE_HWLOC_LIB || USE_HWLOC_UTILS
  // Define these early to avoid harmlss goto-bypasses-initialization warnings
  gasneti_hwloc_obj_type_t type = (gasneti_hwloc_obj_type_t)0;
  gasneti_hwloc_cpuset_t cpuset = NULL;
  int topo_is_init = 0;
#endif

  char *suffix = NULL;
  char *result = NULL;

  // Step 1 - check for suffixed vars
  char *firstkey = check_suffixed(keyname);
  if (! firstkey) {
    // short-cut w/o using hwloc if there are no suffixed variables
    goto out_return_unsuffixed;
  }

  // Step 2 - check env var "[keyname]_TYPE" for "None" (which disables all additional intelligence)
  char *typekey = gasneti_sappendf(NULL, "%s_TYPE", keyname);
  const char *typestring = gasneti_getenv_withdefault(typekey, dflt_type);
  const char *orig_typestring = typestring;
  gasneti_free(typekey);
  if (typestring) {
    if (! gasneti_strcasecmp("none", typestring)) {
      // short-cut w/o using hwloc if TYPE is "none"
      gasneti_free(firstkey);
      goto out_return_unsuffixed;
    }
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

  // Step 6a - query the current proc's cpu binding
  #if USE_HWLOC_LIB
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

  // Step 6b - compute intersection between 'cpuset' and object(s) of 'type'
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
        suff_set_insert(i, div, mod);
      }
    }
    suffix = suff_set_to_string();
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
  gasneti_free(suffix);

out:
  if (typestring != orig_typestring) gasneti_free((void*)typestring);
  suff_set_free();
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
