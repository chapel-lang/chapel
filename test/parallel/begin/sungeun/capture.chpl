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

{
  var i = 0;
  while i < n {
      begin work(i);
      i += 1;
  }
}
