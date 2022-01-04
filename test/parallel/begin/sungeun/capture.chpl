config const n = 250;
var count: sync int = n;
var barrier: single bool;

proc work(i) {
  var c: int = count.readFE()-1;
  if c != 0 {
    count.writeEF(c);
    barrier.readFF();
  } else {
    barrier.writeEF(true);
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
