use Barriers;

proc main() {
  var barrier = new Barrier(3);

  // show that a task moved via an on-stmt will decrement the local runningTask counter
  sync {
    // semi-hacky locale private var so that we don't have to wait on something
    // that lives on locale 0 since that would create an additional task
    pragma "locale private" var mySpin$: sync bool;

    begin {
      mytask();

      // wait for 2nd task to migrate (if it'll actually migrate)
      if numLocales > 1 then while (here.runningTasks() != 2) { }
      writeln("\n", here.runningTasks());
      on Locales[1%numLocales] do mySpin$ = true;
      barrier.barrier();
    }
    begin {
      mytask();

      on Locales [1%numLocales] do mySpin$;
      barrier.barrier();
    }

    mytask();
    barrier.barrier();
  }



  proc mytask() {
    barrier.barrier();
    writeln(here.runningTasks());
    barrier.barrier();
  }
}
