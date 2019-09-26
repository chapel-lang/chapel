// See also test/classes/nilability/assignment-to-owned-from-nil.chpl

class Int {
  var x: int;
}

class Counter {
  var x: int = 0;
  var limit: int;

  proc init(limit: int) {
      this.limit = limit;
  }

  // Return nil if limit is reached.
  proc inc(): owned Int? {
    if x == limit { return nil; }
    x += 1;
    return new owned Int(x);
  } 
}

proc main() {
  var counter = new owned Counter(4);
  var i: owned Int? = counter.inc();

  while i != nil {
    writeln(i);
    i = counter.inc();
  }

  writeln("got nil");
}
