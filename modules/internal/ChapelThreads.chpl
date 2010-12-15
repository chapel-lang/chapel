// IMPORTANT NOTE: This module is currently initialized manually in
// runtime/src/main.c and then again during the normal module
// initialization phase, and therefore must only contain
// initialization code that is safe to call early and twice.
//

// maxThreadsPerLocale the maximum number of threads that can be live
// at any given time
//
// NOTE: The current implementation assumes that all locales will return
// the same values for chpl_comm_getMaxThreads() and 
// CHPL_THREADS_GETMAXTHREADS().  If/when this is no longer a safe
// assumption, this variable should be replaced with a 0 sentinel value
// and the locale should support a maxThreads query to find out what
// the actual limit per locale is.  Arguably this could/should be done
// now, but I'm not prepared to take it on right now.
//
config const maxThreadsPerLocale: int = __primitive("chpl_maxThreads");
const maxThreadsPerLocaleLimit: int = __primitive("chpl_maxThreadsLimit");
if maxThreadsPerLocale < 0 {
  halt("maxThreadsPerLocale must be >= 0");
}
if maxThreadsPerLocaleLimit != 0 {
  if (maxThreadsPerLocale > maxThreadsPerLocaleLimit) then
    __primitive("chpl_warning",
                "specified value of " + maxThreadsPerLocale
                  + " for maxThreadsPerLocale is too high; limit is " + maxThreadsPerLocaleLimit);
  else if (maxThreadsPerLocale == 0) then
    __primitive("chpl_warning",
                "maxThreadsPerLocale is unbounded; however, the limit is " + maxThreadsPerLocaleLimit);
}

//
// the size of a call stack
//
config const callStackSize: uint(64) = 0;
const callStackSizeLimit: uint(64) = __primitive("chpl_callStackSizeLimit");

if callStackSizeLimit != 0 {
  if (callStackSize > callStackSizeLimit) then
    __primitive("chpl_warning",
                "specified value of " + callStackSize
                  + " for callStackSize is too high; limit is " + callStackSizeLimit);
}
