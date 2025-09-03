use ChapelLocks;
use CTypes;
use Map;

config const hi: int = 2**16;

proc test0() {
  var g = new chpl_lockGuard(int);

  // Confirm that default initialization occurs and 'x' is '0'.
  manage g.read() as x do assert(x == 0);
  manage g.write() as x do assert(x == 0);
  assert(g.unsafeAccess() == 0);

  // Add one.
  manage g.write() as x do x += 1;
  assert(g.unsafeAccess() == 1);

  // Add one.
  manage g.write() as x do x += 1;
  assert(g.unsafeAccess() == 2);
}

proc test1() {
  var g1 = new chpl_lockGuard(int);
  var g2: atomic int;

  assert(g1.unsafeAccess() == 0);
  assert(g2.read() == 0);

  forall i in 1..<hi {
    manage g1.write() as x do x += i;
    g2.add(i);
  }

  manage g1.write() as x do
    assert(x == g2.read());
}

proc test2() {
  var g = new chpl_lockGuard(map(int, int));

  // The guard can be used straight up in a 'forall' because its default
  // task intent is 'ref', making it safe to use in a parallel context.
  forall i in 1..<hi do
    manage g.write() as m do m.add(i, 0);

  // Fine because 'm' is 'const ref' due to 'read()', so it can be used
  // within the forall loop after applying task intents. If we were to
  // call 'g.write()', then 'm' would still be 'const ref' in the loop body
  // due to the default task intent. If we were to then try and call methods
  // like 'map.add(...)' the compiler would complain about 'm' being 'const',
  // see 'LockGuardBadTaskIntent.chpl'.
  manage g.read() as m do
    forall i in 1..<hi do assert(m.contains(i));
}

proc main() {
  test0();
  test1();
  test2();
}
