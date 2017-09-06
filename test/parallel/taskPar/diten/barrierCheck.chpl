use Barriers;

config const nTasks = 4;
config const printSpinCount = false;

var b = new Barrier(nTasks);

coforall i in 1..nTasks {
  if i < nTasks {
    b.notify();
  } else {
    var spinCount = 0;
    writeln(b.check()); // false
    b.notify();

    /* wait until all tasks have notified */
    while !b.check() {
      spinCount += 1;
    }
    writeln(b.check()); // true
    if printSpinCount then
      writeln(spinCount);
  }
  b.wait();
}
