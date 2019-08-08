use Barriers;

config const tasksPerLoc = 4;
var taskCounts: [0..#numLocales, 0..#tasksPerLoc] (int, int);

proc main() {
  coforall loc in Locales do on loc {
    var barrier = new Barrier(tasksPerLoc);
    coforall tid in 0..#tasksPerLoc {
      barrier.barrier();
      const taskID = (loc.id * tasksPerLoc) + tid;
      const rt = here.runningTaskCounter.read();
      barrier.barrier();
      taskCounts[loc.id, tid] = (taskID, rt);
    }
  }
  for taskCount in taskCounts do 
    writeln("TASK ", taskCount(1), ": running = ", taskCount(2));
}
