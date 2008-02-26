use Time;

config const numThreads = 15000;
var total: int,
    count: int = numThreads,
     done: sync bool,
    ready: sync bool;

def foo (x) {

  if ready {
    total += x;
    count -= 1;
    if count == 0 then done = true;
    ready = true;
  }
    
}

writeln ("need a short nap ...");
begin
  coforall i in 1..numThreads do
    foo (i);
sleep (10);
ready = true;
if done then writeln ("total is ", total);
