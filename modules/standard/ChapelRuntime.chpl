use ChapelIO;
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

def chpldev_taskTable_add(
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

def chpldev_taskTable_remove(threadID : uint(64))
{
    chpldev_taskTable[threadID].pop();
}

def chpldev_taskTable_print() {
    for thread in chpldev_taskTableD {
        for task in chpldev_taskTable[thread] {
            stderr.writeln("- ", task.filename, ":", task.lineno,
                " on ", thread);
        }
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
    chpldev_taskTable_print();
}

