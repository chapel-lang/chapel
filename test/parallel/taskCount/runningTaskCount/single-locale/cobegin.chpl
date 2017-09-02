use Barriers;

// TODO this is current a bad test because we have no way of ensuring the
// taskCount decrement in waitEndCount will run by the time mytask runs.
proc main() {
  var barrier = new Barrier(4);

  // simple cobegin
  cobegin {
    mytask();
    mytask();
    mytask();
    mytask();
  }
  writeln();


  // nested cobegin
  cobegin {
    cobegin {
      mytask();
      mytask();
    }
    cobegin {
      mytask();
      mytask();
    }
  }



  proc mytask() {
    barrier.barrier();
    writeln(here.runningTasks());
    barrier.barrier();
  }
}
