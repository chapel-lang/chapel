config const numThreads = 64;

var flag$: [0..numThreads] sync bool;

flag$[numThreads] = true;
coforall i in 1..numThreads {
  const tmp = flag$[i];
  writeln("Task ", i, " fired");
  flag$[i-1] = true;
}
