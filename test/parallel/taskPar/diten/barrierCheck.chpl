use Barriers;

config const nTasks = 4;
config const printSpinCount = false;

proc check(b: Barrier) {
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
}

var b = new Barrier(nTasks, BarrierType.Atomic);
var sb = new Barrier(nTasks, BarrierType.Sync);

check(b);
check(sb);
