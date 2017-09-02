var taskCounts: [0..#numLocales] string;

proc main() {
  coforall loc in Locales do on loc {
    const rt = here.runningTasks();
    taskCounts[loc.id] = "TASK " + loc.id + ": running = "+ rt;
  }
  for taskCount in taskCounts do writeln(taskCount);
}
