var taskCounts: [0..#numLocales] (int, int);

proc main() {
  coforall loc in Locales with (ref taskCounts) do on loc {
    const rt = here.runningTasks();
    taskCounts[loc.id] = (loc.id, rt);
  }
  for taskCount in taskCounts do
    writeln("TASK ", taskCount(0), ": running = ", taskCount(1));
}
