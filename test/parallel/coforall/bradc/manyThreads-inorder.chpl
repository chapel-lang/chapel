config const numThreads = 512;

var flag$: [0..numThreads] sync bool;
var alldone$: sync bool;

flag$[0] = true;
coforall i in 1..numThreads {
  const tmp = flag$[i-1];
  writeln("Task ", i, " fired");
  flag$[i] = true;
}
