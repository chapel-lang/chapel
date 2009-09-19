use ChapelBase;
use ChapelIO;
use ChapelTuple;
use ChapelRange;
use ChapelArray;
use DefaultArithmetic;
use DefaultAssociative;
use Containers;

/* 
   This code keeps track of thread identifiers, along with the current
   running and pending tasks and where these tasks spawned from.

   The thread ID information is used for thread teardown at the end of
   program execution.

   The taks information is used to generate a task report (optionally
   displayed when the user Ctrl+C's out of the program).

   The type chpl_threadID_t is a primitve type defined in the include
   files for each threading layer.

*/

// Represents a currently running task
record chpldev_Task {
    var lineno    : uint(32);
    var filename  : string;
}

var chpldev_taskTableD : domain(chpl_threadID_t);
var chpldev_taskTable : [chpldev_taskTableD] Vector(chpldev_Task);

pragma "export" def chpldev_taskTable_addThread( threadID : chpl_threadID_t )
{
  if(!chpldev_taskTableD.member(threadID)) {
    chpldev_taskTableD.add(threadID);
  }
}

pragma "export" def chpldev_taskTable_add(
    threadID : chpl_threadID_t,
    lineno   : uint(32),
    filename : string)
{

  // make sure this thread ID is in the domain.
  chpldev_taskTable_addThread(threadID);

  // make sure a task vector exits for this thread ID.
  if (chpldev_taskTable[threadID] == nil) {
    chpldev_taskTable[threadID] = new Vector(chpldev_Task);
  }

  // add task to table; set it to be pending
  chpldev_taskTable[threadID].push(
    new chpldev_Task(lineno, filename));
}

pragma "export" def chpldev_taskTable_remove(threadID : chpl_threadID_t)
{
    chpldev_taskTable[threadID].pop();
}

pragma "export" def chpldev_taskTable_print() 
{
  for threadID in chpldev_taskTableD {
    for task in chpldev_taskTable[threadID] {
      stderr.writeln("- ", task.filename, ":", task.lineno,
                     " on ", threadID : uint(64));
     }
  }
}


// callbacks to C part of runtime for thread cleanup.
_extern def chpl_thread_cancel(threadID : chpl_threadID_t );
_extern def chpl_thread_join  (threadID : chpl_threadID_t );


pragma "export" def chpldev_endAllThreads() : void
{
  for thread in chpldev_taskTableD do
    chpl_thread_cancel(thread);
  for thread in chpldev_taskTableD do
    chpl_thread_join(thread);
}
    
