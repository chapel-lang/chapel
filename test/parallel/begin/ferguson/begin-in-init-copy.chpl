class Impl {
  var x:int;
}

record R {
  var ptr:unmanaged Impl;
}

proc R.init() {
  this.ptr = new unmanaged Impl(0);
}

proc R.deinit() {
  delete this.ptr;
}

proc R.init=(from:R) {
  this.ptr = new unmanaged Impl(from.ptr.x);
}

proc =(ref lhs:R, rhs:R) {
  lhs.ptr = new unmanaged Impl(rhs.ptr.x);
}

proc R.increment() {
  this.ptr.x += 1;
}


config const n = 10000;
config const debug = false;

proc report(phase, hits) {

  var mismatches = 0;
  for i in 0..#n {
    var count = hits[i].read();
    if count != 1 {
      mismatches += 1;
      if debug then
        writeln(phase, " hits count ", i, " is ", count, " not 1!");
    }
  }

  writeln(phase, " has ", mismatches, " mismatches");
}

proc test() {

  {
    var hits:[0..n] atomic int;

    var r:R;

    // First, run the pattern in a serial loop
    for i in 0..#n {
      hits[min(n,r.ptr.x)].add(1);
      r.increment();
    }

    // Report any hits != 1
    report("serial", hits);
  }

  {
    var hits:[0..n] atomic int;

    var r:R;

    // Now, run it with a begin per iteration
    sync {
      for i in 0..#n {
        begin with (in r) {
          hits[min(n,r.ptr.x)].add(1);
        }
        r.increment();
      }
    }

    // Report any hits != 1
    report("begins", hits);
  }
}

test();
