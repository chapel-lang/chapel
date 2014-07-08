var num: atomic int;
coforall loc in Locales do on loc {
  num.add(here.runningTasks());
}

if (num.read() > numLocales) {
  writeln("Error: there was (were) ", num.read() - numLocales, " additional task(s) created");
} else {
  writeln("Success: each locale had only one task");
 }
