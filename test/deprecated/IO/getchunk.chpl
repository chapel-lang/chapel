
use IO;

proc main() {
  var f = open("getchunk.txt", iomode.r);

  const (lo, hi) = f.getchunk();
  writeln("lo = ", lo);
  writeln("hi = ", hi);
}
