
use IO;

record R {
  var D : domain(1);
  var A : [D] int;
}

proc main() {
  var r = new R({1..10});
  r.A = 1..10;

  var f = openMemFile();
  f.writer().write(r);
  var x = f.reader().read(R);
  writeln(r == x);
}
