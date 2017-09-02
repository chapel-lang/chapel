config const tasksPerLoc = 4;

proc main() {
  for loc in Locales do on loc {
    for tid in 0..#tasksPerLoc {
      const taskID = (loc.id * tasksPerLoc) + tid;
      const rt = here.runningTasks();
      writeln("TASK ", taskID, ": running = ", rt);
    }
  }
}
