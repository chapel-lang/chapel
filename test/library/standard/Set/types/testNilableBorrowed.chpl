import Set.set;

class T {
  var value = 0;
}

// Copy to work around weird type instantiation bug.
proc testSetOfBorrowed() {
  var s = new set(borrowed T);

  var x = new borrowed T(1);

  s.add(x);
  assert(s.size == 1);

  s.remove(x);
  assert(s.size == 0);
}

testSetOfBorrowed();
