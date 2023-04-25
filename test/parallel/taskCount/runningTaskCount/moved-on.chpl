use Collectives;

// Semi-hacky locale private var so that we don't have to wait on something
// that lives on locale 0 since that would create an additional task. Note that
// we need to modify/read through functions to avoid task-intents

pragma "locale private"
var doneSpinning: chpl__processorAtomicType(bool);
proc setDoneSpinning() { doneSpinning.write(true); }
proc spin() { doneSpinning.waitFor(true); }

proc main() {
  var bar = new barrier(3);

  // show that a task moved via an on-stmt will decrement the local runningTask counter
  sync {
    begin {
      mytask();

      // wait for 2nd task to migrate (if it'll actually migrate)
      if numLocales > 1 then while (here.runningTasks() != 2) { chpl_task_yield(); }
      writeln("\n", here.runningTasks());
      on Locales[1%numLocales] do setDoneSpinning();
      bar.barrier();
    }
    begin {
      mytask();

      on Locales [1%numLocales] do spin();
      bar.barrier();
    }

    mytask();
    bar.barrier();
  }



  proc mytask() {
    bar.barrier();
    writeln(here.runningTasks());
    bar.barrier();
  }
}
