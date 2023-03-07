use Collectives;

config const nTasks = 4;

proc copyInBar(in bar: barrier) {
  coforall tid in 1..nTasks {
    bar.barrier();
  }
  writeln("first coforall done!");
}

var bar = new barrier(nTasks);

copyInBar(bar);

coforall tid in 1..nTasks {
  bar.barrier();
}

writeln("second coforall done!");

proc varCopyBar(bar: barrier) {
  var x = bar;
  coforall tid in 1..nTasks {
    if tid % 2 == 0 then
      bar.barrier();
    else
      x.barrier();
  }
  writeln("third coforall done!");
}

varCopyBar(bar);

coforall tid in 1..nTasks {
  bar.barrier();
}

writeln("fourth coforall done!");
