use Barriers;

config const tasksPerLoc = 4;

proc main() {
  // 2 cobegins spawn coforalls that spawn tasksPerLoc tasks, each of which
  // spawn an additional task:
  var barrier = new Barrier(2 * (tasksPerLoc * 2));

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
    barrier.barrier();
    writeln(here.runningTasks());
    barrier.barrier();
  }
}

