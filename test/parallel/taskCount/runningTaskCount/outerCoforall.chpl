var taskCounts: [0..#numLocales] (int, int);

proc main() {
  coforall loc in Locales do on loc {
    const rt = here.runningTasks();
    taskCounts[loc.id] = (loc.id, rt);
  }
  for taskCount in taskCounts do
    writeln("TASK " + taskCount(1) + ": running = " + taskCount(2));
}
