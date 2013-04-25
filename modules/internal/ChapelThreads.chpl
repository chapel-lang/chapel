// ChapelThreads.chpl
//
// IMPORTANT NOTE: This module is currently initialized manually in
// runtime/src/main.c and then again during the normal module
// initialization phase, and therefore must only contain
// initialization code that is safe to call early and twice.
//
// numThreadsPerLocale is used to tell the task layer how many threads
// to use to execute the user's tasks.  The interpretation of the
// value varies depending on task layer and is documented in README.tasks
// in the release documentation directory.  The sentinel value of 0
// indicates that the tasking layer can determine the number of
// threads to use.
//
pragma "no use ChapelStandard"
pragma "export init"
module ChapelThreads {
  extern proc chpl_task_getenvNumThreadsPerLocale(): int(32);
  pragma "export"
  config const numThreadsPerLocale: int = chpl_task_getenvNumThreadsPerLocale();

  extern proc chpl_task_getenvCallStackSize(): int(64);
  pragma "export"
  config const callStackSize: int = chpl_task_getenvCallStackSize();
}
