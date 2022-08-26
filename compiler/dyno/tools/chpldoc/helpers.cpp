#include "helpers.h"
#include "chpl/util/filesystem.h"

bool developer = false;


bool startsWith(const char* str, const char* prefix) {
  return (0 == strncmp(str, prefix, strlen(prefix)));
}

void clean_exit(int status) {
  exit(status);
}

void setupError(std::string msg) {
  printf("setupError %s", msg.c_str());
}


uint64_t hexStr2uint64(const char* str, bool userSupplied,
                       const char* filename, int line) {
  if (!str) {
    INT_FATAL("NULL string passed to hexStrToUint64()");
  }
  int len = strlen(str);
  if (len < 3 || str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
    INT_FATAL("Illegal string passed to hexStrToUint64()");
  }
  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  if (strlen(str+startPos) > 16) {
    if (userSupplied) {
      //astlocT astloc(line, filename);
      USR_FATAL("Integer literal overflow: '" + std::string(str) + "' is too big "
                "for type 'uint64'");
    } else {
      INT_FATAL("Integer literal overflow: '" + std::string(str) + "' is too big "
                "for type 'uint64'");
    }
  }

  uint64_t val;
  int numitems = sscanf(str+2, "%" SCNx64, &val);
  if (numitems != 1) {
    INT_FATAL("Illegal string passed to hexStrToUint64");
  }
  return val;
}

// // would just use realpath, but it is not supported on all platforms.
static
char* chplRealPath(const char* path)
{
  // We would really rather use
  // char* got = realpath(path, NULL);
  // but that doesn't work on some Mac OS X versions.
  char* buf = (char*) malloc(1024);
  char* got = realpath(path, buf);
  char* ret = NULL;
  if( got ) ret = strdup(got);
  free(buf);
  return ret;
}


// Returns a "real path" to the file in the directory,
// or NULL if the file did not exist.
// The return value must be freed by the caller.
// We try to use realpath but might give up.
static
char* dirHasFile(const char *dir, const char *file)
{
  struct stat stats;
  int len = strlen(dir) + strlen(file) + 2;
  char* tmp = (char*) malloc(len);
  char* real;

  if( ! tmp ) INT_FATAL("no memory");

  snprintf(tmp, len, "%s/%s", dir, file);
  real = chplRealPath(tmp);
  if( real == NULL ) {
    // realpath not working on this system,
    // just use tmp.
    real = tmp;
  } else {
    free(tmp);
  }

  if( stat(real, &stats) != 0) {
    free(real);
    real = NULL;
  }

  return real;
}


// Find the path to the running program
// (or return NULL if we couldn't figure it out).
// The return value must be freed by the caller.
char* findProgramPath(const char *argv0)
{
  char* real = NULL;
  char* path;

  /* Note - there are lots of friendly
   * but platform-specific ways to do this:
    #ifdef __linux__
      int ret;
      ret = readlink("/proc/self/exe", dst, max_dst - 1);
      // return an error if there was an error.
      if( ret < 0 ) return -1;
      // append the NULL byte
      if( ret < max_dst ) dst[ret] = '\0';
      return 0;
    #else
    #ifdef __APPLE__
      uint32_t sz = max_dst - 1;
      return _NSGetExecutablePath(dst, &sz);
    #else
      // getexe path not available.
      return -1;
    #endif
  */


  // Is argv0 an absolute path?
  if( argv0[0] == '/' ) {
    real = dirHasFile("/", argv0);
    return real;
  }

  // Is argv0 a relative path?
  if( strchr(argv0, '/') != NULL ) {
    std::string cwd;
    if(auto err = chpl::currentWorkingDir(cwd)) {
      real = NULL;
    } else {
      real = dirHasFile(cwd.c_str(), argv0);
    }
    return real;
  }

  // Is argv0 just in $PATH?
  path = getenv("PATH");
  if( path == NULL ) return NULL;

  path = strdup(path);
  if( path == NULL ) return NULL;

  // Go through PATH changing ':' into '\0'
  char* start;
  char* end;
  char* path_end = path + strlen(path);

  start = path;
  while( start != NULL && start < path_end ) {
    end = strchr(start, ':');
    if( end == NULL ) end = path_end;
    else end[0] = '\0'; // replace ':' with '\0'

    real = dirHasFile(start, argv0);
    if( real ) break;

    start = end + 1;
  }

  free(path);
  return real;
}



#define define_str2Int(type, format)                              \
  type##_t str2##type(const char* str,                            \
                      bool userSupplied,                          \
                      const char* filename,                       \
                      int line) {                                 \
    if (!str) {                                                   \
      INT_FATAL("NULL string passed to strTo_" #type "()");       \
    }                                                             \
    int len = strlen(str);                                        \
    if (len < 1) {                                                \
      INT_FATAL("empty string passed to strTo_" #type "()");      \
    }                                                             \
    type##_t val;                                                 \
    int numitems = sscanf(str, format, &val);                     \
    char checkStr[len+1];                                         \
    snprintf(checkStr, len+1, format, val);                       \
    if (numitems != 1) {                                          \
      INT_FATAL("Illegal string passed to strTo_" #type "()");    \
    }                                                             \
    /* Remove leading 0s */                                       \
    int startPos = 0;                                             \
    while (str[startPos] == '0' && startPos < len-1) {            \
      startPos++;                                                 \
    }                                                             \
    if (strcmp(str+startPos, checkStr) != 0) {                    \
      if (userSupplied) {                                         \
                                  \
        USR_FATAL("Integer literal overflow: '" + std::string(str) + "' is too" \
                  " big for type '" #type "'");              \
      } else {                                                    \
        INT_FATAL("Integer literal overflow: '" + std::string(str) + "' is too "        \
                  "big for type '" #type "'");               \
      }                                                           \
    }                                                             \
    return val;                                                   \
  }


define_str2Int(int64, "%" SCNd64)

define_str2Int(uint64, "%" SCNu64)

