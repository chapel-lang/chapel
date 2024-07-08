use Collectives;

proc main() {
  var bar = new barrier(4);

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
    bar.barrier();
    writeln(here.runningTasks());
    bar.barrier();
  }
}
