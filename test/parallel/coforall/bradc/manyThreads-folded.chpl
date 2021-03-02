config const numThreads = 64;
config const printExits = true;

var flag$: [0..numThreads] sync bool;
var alldone$: sync bool;

flag$[0].writeEF(true);
coforall i in 1..numThreads {
  const tmp = flag$[i-1].readFE();
  writeln("Task ", i, " fired");
  flag$[i].writeEF(true);
  if (i == numThreads) {
    writeln("--------");
    alldone$.writeEF(true);
  } else {
    const tmp = alldone$.readFF();
  }
  if printExits then writeln("Task ", i, " exiting");
}
