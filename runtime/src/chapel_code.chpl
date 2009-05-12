use ChapelBase;
use ChapelIO;
use ChapelTuple;
use ChapelRange;
use ChapelArray;
use DefaultArithmetic;
use DefaultAssociative;
use Containers;
use Sort;

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


// keeps track of the total memory allocated for a given description,
// and how many items fit that description
record __mem_alloc {
    var number    : uint(64);
    var totalSize : uint(64);
}

var __mem_allocTableD : domain(string);
var __mem_allocTable : [__mem_allocTableD] __mem_alloc;

pragma "export" def __mem_alloc_add(
    description : string,
    size : uint(64))
{
    // add description to domain
    if __mem_allocTableD.member(description) {
      __mem_allocTable[description].number += 1;
      __mem_allocTable[description].totalSize += size;
    } else {
      __mem_allocTableD.add(description);
      __mem_allocTable[description] = new __mem_alloc(1, size);
    }
}

pragma "export" def __mem_alloc_print(formatString: string) {
  var count = 0;
  for description in __mem_allocTableD do
    count += 1;

  record memAlloc {
    var number, totalSize: uint(64);
    var description: string;
  }
  var sortedMemAllocTable: [1..count] memAlloc;

  for (memAllocEntry, description) in (sortedMemAllocTable, __mem_allocTableD) {
    memAllocEntry.number      = __mem_allocTable[description].number;
    memAllocEntry.totalSize   = __mem_allocTable[description].totalSize;
    memAllocEntry.description = description;
  }

  // sort in reverse order
  def <(m1, m2) { return m1.totalSize > m2.totalSize; }
  QuickSort(sortedMemAllocTable);

  for memAllocEntry in sortedMemAllocTable {
    writeln(format(formatString, memAllocEntry.number), "  ",
            format(formatString, memAllocEntry.totalSize), "  ",
            memAllocEntry.description);
  }
}
