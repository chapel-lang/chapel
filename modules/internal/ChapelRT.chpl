use ChapelBase;
use ChapelIO;
use ChapelArray;
use DefaultRectangular;
use DefaultAssociative;

/* 
   This code keeps track of tasks, their state, and where they
   spawned from.

   The taks information is used to generate a task report (optionally
   displayed when the user Ctrl+C's out of the program).

   The type chpl_taskID_t is a primitve type defined in the include
   files for each tasking layer.

*/

enum taskState { pending, active, suspended };

//
// This represents a currently running task.  The state, lineno, and
// filename members should be obvious; tl_info is information belonging
// to the tasking layer.
//
// If we included the parent in this, the print method could topologically
// sort the tasks and indent to show the parent/child relationships, which
// might be very nice.
//
record chpldev_Task {
  var state     : taskState;
  var lineno    : uint(32);
  var filename  : string;
  var tl_info   : uint(64);
}

var chpldev_taskTableD : domain(chpl_taskID_t, parSafe=false);
var chpldev_taskTable : [chpldev_taskTableD] chpldev_Task;

export proc chpldev_taskTable_add(taskID   : chpl_taskID_t,
                                  lineno   : uint(32),
                                  filename : string,
                                  tl_info  : uint(64))
{
  if(!chpldev_taskTableD.member(taskID)) {
    // This must be serial to avoid deadlock in a coforall. <hilde>
    serial true do
      chpldev_taskTableD.add(taskID);
  }
  chpldev_taskTable[taskID] = new chpldev_Task(taskState.pending,
                                               lineno, filename, tl_info);
}

export proc chpldev_taskTable_remove(taskID : chpl_taskID_t)
{
  // This must also be serial
  serial true do
    chpldev_taskTableD.remove(taskID);
}

export proc chpldev_taskTable_set_active(taskID : chpl_taskID_t)
{
  chpldev_taskTable[taskID].state = taskState.active;
}

export proc chpldev_taskTable_set_suspended(taskID : chpl_taskID_t)
{
  chpldev_taskTable[taskID].state = taskState.suspended;
}

export proc chpldev_taskTable_get_tl_info(taskID : chpl_taskID_t)
{
  return chpldev_taskTable[taskID].tl_info;
}

export proc chpldev_taskTable_print() 
{
  for taskID in chpldev_taskTableD {
    stderr.writeln("- ", chpldev_taskTable[taskID].filename,
                   ":",  chpldev_taskTable[taskID].lineno,
                   " is ", chpldev_taskTable[taskID].state);
  }
}
