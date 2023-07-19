use Collectives;

config const tasksPerLoc = 4;
var taskCounts: [0..#numLocales, 0..#tasksPerLoc] (int, int);

proc main() {
  for loc in Locales do on loc {
    var bar = new barrier(tasksPerLoc);
    coforall tid in 0..#tasksPerLoc {
      bar.barrier();
      const taskID = (loc.id * tasksPerLoc) + tid;
      const rt = here.runningTasks();
      bar.barrier();
      taskCounts[loc.id, tid] = (taskID, rt);
    }
  }
  for taskCount in taskCounts do
    writeln("TASK ", taskCount(0), ": running = ", taskCount(1));
}
