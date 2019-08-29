config const trials = 10000;
for t in 1..trials {
  coforall loc in Locales do on loc {
    const rt = here.runningTasks();
    if rt > 1 {
      writeln("Error trial ", t, " on locale ", loc.id, " has ", rt, " tasks");
    }
  }
}
