use Collectives;

config const tasksPerLoc = 4;

proc main() {
  // 2 cobegins spawn coforalls that spawn tasksPerLoc tasks, each of which
  // spawn an additional task:
  var bar = new barrier(2 * (tasksPerLoc * 2));

  cobegin {
    {
      coforall 1..tasksPerLoc {
        sync {
          begin { mytask(); }
          mytask();
        }
      }
    }
    {
      coforall 1..tasksPerLoc {
        sync {
          begin { mytask(); }
          mytask();
        }
      }
    }
  }



  proc mytask() {
    bar.barrier();
    writeln(here.runningTasks());
    bar.barrier();
  }
}

