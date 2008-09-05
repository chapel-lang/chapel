use ChapelBase;
use ChapelIO;
use ChapelTuple;
use ChapelRange;
use ChapelArray;
use DefaultArithmetic;
use DefaultAssociative;
use Containers;

/* The following deals with the task table structure. The task table is
   responsible for keeping track of the current running and pending tasks
   and where these tasks spawned from. This information is used to generate
   a task report (optionally displayed when the user Ctrl+C's out of 
   the program).
*/

// Represents a currently running task
record chpldev_Task {
    var lineno    : uint(32);
    var filename  : string;
}

var chpldev_taskTableD : domain(uint(64));
var chpldev_taskTable : [chpldev_taskTableD] Vector(chpldev_Task);

pragma "export" def chpldev_taskTable_add(
    threadID : uint(64),
    lineno   : uint(32),
    filename : string)
{
    // add task to table set it to be pending
    if(!chpldev_taskTableD.member(threadID)) {
        chpldev_taskTableD.add(threadID);
        chpldev_taskTable[threadID] =
            new Vector(chpldev_Task);
    }
    chpldev_taskTable[threadID].push(
        new chpldev_Task(lineno, filename));
}

pragma "export" def chpldev_taskTable_remove(threadID : uint(64))
{
    chpldev_taskTable[threadID].pop();
}

pragma "export" def chpldev_taskTable_print() {
    for thread in chpldev_taskTableD {
        for task in chpldev_taskTable[thread] {
            stderr.writeln("- ", task.filename, ":", task.lineno,
                " on ", thread);
        }
    }
}
