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

class chpldev_taskTable_t {
  var dom : domain(chpl_taskID_t, parSafe=false);
  var map : [dom] chpldev_Task;
}

pragma "private"
var chpldev_taskTable : chpldev_taskTable_t;

//----------------------------------------------------------------------{
//- Code to initialize the task table on each locale.
//-
proc chpldev_taskTable_init() {
  for locid in 0..numLocales-1 do
    on __primitive("chpl_on_locale_num", locid) {
      // Task tables require that the local default distribution be initialized first.
      if (defaultDist._value == nil) then
        defaultDist = new dmap(new DefaultDist());
      chpldev_taskTable = new chpldev_taskTable_t();
    }
}

chpldev_taskTable_init();
//-
//----------------------------------------------------------------------}

export proc chpldev_taskTable_add(taskID   : chpl_taskID_t,
                                  lineno   : uint(32),
                                  filename : string,
                                  tl_info  : uint(64))
{
  if (chpldev_taskTable == nil) then return;

  if (!chpldev_taskTable.dom.member(taskID)) then
    // This must be serial to avoid deadlock in a coforall. <hilde>
    serial true do
      chpldev_taskTable.dom.add(taskID);

  chpldev_taskTable.map[taskID] = new chpldev_Task(taskState.pending,
                                               lineno, filename, tl_info);
}

export proc chpldev_taskTable_remove(taskID : chpl_taskID_t)
{
  if (chpldev_taskTable == nil) then return;

  if (chpldev_taskTable.dom.member(taskID)) then
    // This must also be serial
    serial true do
      chpldev_taskTable.dom.remove(taskID);
}

export proc chpldev_taskTable_set_active(taskID : chpl_taskID_t)
{
  if (chpldev_taskTable == nil) then return;

  chpldev_taskTable.map[taskID].state = taskState.active;
}

export proc chpldev_taskTable_set_suspended(taskID : chpl_taskID_t)
{
  if (chpldev_taskTable == nil) then return;

  chpldev_taskTable.map[taskID].state = taskState.suspended;
}

export proc chpldev_taskTable_get_tl_info(taskID : chpl_taskID_t)
{
  if (chpldev_taskTable == nil) then return 0:uint(64);

  return chpldev_taskTable.map[taskID].tl_info;
}

export proc chpldev_taskTable_print() 
{
  if (chpldev_taskTable == nil) then return;

  for taskID in chpldev_taskTable.dom {
    stderr.writeln("- ", chpldev_taskTable.map[taskID].filename,
                   ":",  chpldev_taskTable.map[taskID].lineno,
                   " is ", chpldev_taskTable.map[taskID].state);
  }
}
