config const n = 77;

var l: sync bool = false;

coforall i in 1..n {
  l;
  writeln(here.runningTasks());
  l = true;
}

