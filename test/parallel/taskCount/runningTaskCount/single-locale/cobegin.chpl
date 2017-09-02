use Barriers;

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
