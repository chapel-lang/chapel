config const numThreads = 64;
config const printExits = true;

var flag$: [0..numThreads] sync bool;
var alldone$: sync bool;

flag$[0] = true;
coforall i in 1..numThreads {
  const tmp = flag$[i-1];
  writeln("Task ", i, " fired");
  flag$[i] = true;
  if (i == numThreads) {
    writeln("--------");
    alldone$ = true;
  } else {
    const tmp = alldone$.readFF();
  }
  if printExits then writeln("Task ", i, " exiting");
}
