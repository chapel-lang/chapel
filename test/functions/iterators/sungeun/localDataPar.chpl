config const lo = 9;
config const hi = 17;
config const noisy = false;

config const dptpl = 3;
config const ir = true;
config const mg = 19;

const D = {lo..hi, (lo..hi)+1, (lo..hi)+2, (lo..hi)+3};
var A: [D] real;

forall i in D.these(tasksPerLocale=dptpl, ignoreRunning=ir) {
  if noisy then writeln(i);
}

forall i in D.these(minIndicesPerTask=mg, ignoreRunning=ir) {
  if noisy then writeln(i);
}

forall i in A.these(tasksPerLocale=dptpl, ignoreRunning=ir) {
  if noisy then writeln(i);
}

forall i in A.these(minIndicesPerTask=mg, ignoreRunning=ir) {
  if noisy then writeln(i);
}

