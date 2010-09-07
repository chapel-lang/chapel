// the maximum number of threads that can be live at any given time
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
config const callStackSize: uint(64) = __primitive("chpl_callStackSize");
const callStackSizeLimit: uint(64) = __primitive("chpl_callStackSizeLimit");

if callStackSizeLimit != 0 {
  if (callStackSize > callStackSizeLimit) then
    __primitive("chpl_warning",
                "specified value of " + callStackSize
                  + " for callStackSize is too high; limit is " + callStackSizeLimit);
  else if (callStackSize == 0) then
    __primitive("chpl_warning",
                "callStackSize is unbounded; however, the limit is " + callStackSizeLimit);
}
