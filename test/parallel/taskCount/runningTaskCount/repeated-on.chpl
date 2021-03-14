inline proc movedOn() {
  var count: int;
  on Locales [1%numLocales] do count = here.runningTasks();
  writeln(count);
}

for param i in 1..10 do movedOn();
