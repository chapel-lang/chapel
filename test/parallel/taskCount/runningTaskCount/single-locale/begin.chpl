use Collectives; 

proc main() {
  var bar = new barrier(4);

  // simple begin
  sync {
    begin { mytask(); }
    begin { mytask(); }
    begin { mytask(); }
    mytask();
  }

  proc mytask() {
    bar.barrier();
    writeln(here.runningTasks());
    bar.barrier();
  }
}
