use Barriers;

proc main() {
  var barrier = new Barrier(4);

  // simple begin
  begin { mytask(); }
  begin { mytask(); }
  begin { mytask(); }
  mytask();

  proc mytask() {
    barrier.barrier();
    writeln(here.runningTasks());
    barrier.barrier();
  }
}
