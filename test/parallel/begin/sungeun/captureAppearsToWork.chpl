config const n = 2000;
proc work(i) {
  writeln(i);
}

// This *appears* to capture i because it's scope is that of the forall
//  loop and so it is effectively a per iteration const.
forall i in 0..n-1 do begin work(i);
