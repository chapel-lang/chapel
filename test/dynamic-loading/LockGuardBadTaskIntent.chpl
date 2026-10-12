use ChapelLocks;
use CTypes;
use Map;

config const hi = 2**16;

proc test0() {
  var g = new chpl_lockGuard(map(int, int));

  // This should not work because 'm' is 'ref', and its default task intent
  // will be 'const ref', meaning the user has to opt into task-unsafety
  // by writing 'with (ref m)'.
  manage g.write() as m do
    forall i in 1..<hi do m.add(i, 0);
}

proc main() {
  test0();
}
