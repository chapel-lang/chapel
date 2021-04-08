config const numThreads = 64;

var flag$: [0..numThreads] sync bool;

flag$[numThreads].writeEF(true);
coforall i in 1..numThreads {
  const tmp = flag$[i].readFE();
  writeln("Task ", i, " fired");
  flag$[i-1].writeEF(true);
}
