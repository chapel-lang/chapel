use Barrier;

config const nTasks = 4;
config const printSpinCount = false;

var b = new Barrier(nTasks);

coforall i in 1..nTasks {
  if i < nTasks {
    b.notify();
  } else {
    var spinCount = 0;
    writeln(b.try()); // false
    b.notify();

    /* wait until all tasks have notified */
    while !b.try() {
      spinCount += 1;
    }
    writeln(b.try()); // true
    if printSpinCount then
      writeln(spinCount);
  }
  b.wait();
}

delete b;
