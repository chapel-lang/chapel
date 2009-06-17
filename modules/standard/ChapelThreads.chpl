// the maximum number of threads that can be live at any given time
config const maxThreads: int = __primitive("chpl_maxThreads");
const maxThreadsLimit: int = __primitive("chpl_maxThreadsLimit");
if maxThreadsLimit != 0 {
  if (maxThreads > maxThreadsLimit) then
    __primitive("chpl_warning",
                "specified value of " + maxThreads
                  + " for maxThreads is too high; limit is " + maxThreadsLimit);
  else if (maxThreads == 0) then
    __primitive("chpl_warning",
                "maxThreads is unbounded; however, the limit is " + maxThreadsLimit);
}

