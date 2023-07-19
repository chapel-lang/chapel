use Collectives;

config const tasksPerLoc = 4;

proc main() {
  var bar = new barrier(tasksPerLoc);

  // simple coforall
  coforall 1..tasksPerLoc {
    bar.barrier();
    writeln(here.runningTasks());
    bar.barrier();
  }
  writeln();


  // "unbounded" coforall
  coforall unknownTasksWrapperIter() {
    bar.barrier();
    // lame hack to ensure waitEndCount has been called by the main task
    while (here.runningTasks() != tasksPerLoc) { }
    writeln(here.runningTasks());
    bar.barrier();
  }
  writeln();


  var nestedBarrier = new barrier(tasksPerLoc*tasksPerLoc);
  // nested coforall
  coforall 1..tasksPerLoc {
    coforall 1..tasksPerLoc {
      nestedBarrier.barrier();
      writeln(here.runningTasks());
      nestedBarrier.barrier();
    }
  }
}

// dummy iter to thwart bounded-coforall optimization
iter unknownTasksWrapperIter() {
  for i in 1..tasksPerLoc do yield i;
}
