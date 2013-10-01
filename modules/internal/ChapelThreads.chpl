// ChapelThreads.chpl
//
// numThreadsPerLocale is used to tell the task layer how many threads
// to use to execute the user's tasks.  The interpretation of the
// value varies depending on task layer and is documented in README.tasks
// in the release documentation directory.  The sentinel value of 0
// indicates that the tasking layer can determine the number of
// threads to use.
//
pragma "no use ChapelStandard"
module ChapelThreads
{
  extern proc chpl_task_getenvNumThreadsPerLocale(): int(32);
  const numThreadsPerLocale: int = chpl_task_getenvNumThreadsPerLocale();

  extern proc chpl_task_getenvCallStackSize(): int(64);
  const callStackSize: int = chpl_task_getenvCallStackSize();
}
