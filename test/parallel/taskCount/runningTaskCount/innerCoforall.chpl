use Barriers;

config const tasksPerLoc = 4;
var taskCounts: [0..#numLocales, 0..#tasksPerLoc] (int, int);

proc main() {
  for loc in Locales do on loc {
    var barrier = new Barrier(tasksPerLoc);
    coforall tid in 0..#tasksPerLoc {
      barrier.barrier();
      const taskID = (loc.id * tasksPerLoc) + tid;
      const rt = here.runningTasks();
      barrier.barrier();
      taskCounts[loc.id, tid] = (taskID, rt);
    }
  }
  for taskCount in taskCounts do
    writeln("TASK ", taskCount(0), ": running = ", taskCount(1));
}
