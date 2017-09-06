use Barriers;

config const tasksPerLoc = 4;

proc main() {
  var barrier = new Barrier(tasksPerLoc);

  // simple coforall
  coforall 1..tasksPerLoc {
    barrier.barrier();
    writeln(here.runningTasks());
    barrier.barrier();
  }
  writeln();


  // "unbounded" coforall
  coforall unknownTasksWrapperIter() {
    barrier.barrier();
    // lame hack to ensure waitEndCount has been called by the main task
    while (here.runningTasks() != tasksPerLoc) { }
    writeln(here.runningTasks());
    barrier.barrier();
  }
  writeln();


  var nestedBarrier = new Barrier(tasksPerLoc*tasksPerLoc);
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
