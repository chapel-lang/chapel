config const n = 250;
var count: sync int = n;
var barrier: single bool;

proc work(i) {
  var c: int = count-1;
  if c != 0 {
    count = c;
    barrier;
  } else {
    barrier = true;
  }
  writeln(i);
}

// This *appears* to capture i because it's scope is that of the forall
//  loop and so it is effectively a per iteration const.
forall i in 0..n-1 do begin work(i);
