use Collectives;
config const n = 250;

var bar = new barrier(n);

proc work(i) {
  bar.barrier();
  writeln(i);
}

{
  var i = 0;
  while i < n {
      begin work(i);
      i += 1;
  }
}
