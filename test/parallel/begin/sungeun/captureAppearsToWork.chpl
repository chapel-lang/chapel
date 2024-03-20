use Collectives;
config const n = 250;
var bar = new barrier(n);

proc work(i) {
  bar.barrier();
  writeln(i);
}

// This *appears* to capture i because it's scope is that of the forall
//  loop and so it is effectively a per iteration const.
forall i in 0..n-1 do begin work(i);
