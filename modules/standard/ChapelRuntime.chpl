use ChapelIO;

/* The following deals with the task table structure. The task table is
   responsible for keeping track of the current running and pending tasks
   and where these tasks spawned from. This information is used to generate
   a task report (optionally displayed when the user Ctrl+C's out of 
   the program).
*/

// Represents a atsk and contains infromation about where it spawned
// and whether its running or pending.
record chpldev_Task {
    var lineno    : uint(32);
    var filename  : string;
    var isRunning : bool;
    var threadID  : uint(64);
}

var chpldev_taskTableD : domain(uint(64));
var chpldev_taskTable : [chpldev_taskTableD] chpldev_Task;

def chpldev_taskTable_add(
    taskID   : uint(64),
    lineno   : uint(32),
    filename : string)
{
    writeln("add task ", taskID, " at ", filename, ":", lineno);

    // add task to table set it to be pending
    chpldev_taskTableD.add(taskID);
    chpldev_taskTable[taskID] =
        new chpldev_Task(lineno, filename, false);
}

def chpldev_taskTable_remove(taskID : uint(64))
{
    writeln("remove task ", taskID);
    chpldev_taskTable.domain.remove(taskID);
}

def chpldev_taskTable_setRunning(taskID : uint(64), threadID : uint(64))
{
    writeln("task ", taskID, " started on thread ", threadID);
    var task = chpldev_taskTable[taskID];
    task.isRunning = true;
    task.threadID = threadID;
}

def chpldev_taskTable_print() {
    for task in chpldev_taskTable {
        writeln(task.filename, ":", task.lineno, " - ", task.isRunning);
    }
}

/* In order to ensure the functions above don't get killed by dead-code
   elimination its necessary to call them from within this module initializer.
   To prevent them from actually being called I wrap them in an if statement
   that's gauranteed to be false but sufficiently non-trivial that it won't
   get dead-code eliminated itself. */
var chpldev_mustBeFalse = false;
if chpldev_mustBeFalse {
    chpldev_taskTable_add(0,0,"");
    chpldev_taskTable_remove(0);
    chpldev_taskTable_setRunning(0,0);
    chpldev_taskTable_print();
}

