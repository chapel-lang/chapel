use Time;

// Multi-threaded test with a record that has a sync var field.

// Part 1. Main thread slowly writes to s.  Child thread reads from s and
// outputs what was read.

param ITERATIONS = 10;

record R {
  var s: sync int;
}

var c: R;
sync {
  writeln( "\ntest 1");
  begin with (ref c) {
    var r: int;
    var j: int;
    j = 0;
    while (j < ITERATIONS) {
      r = c.s;
      writeln( "2: got ", r);
      j += 1;
    }
  }

  var k: int;
  k = 0;
  while (k < ITERATIONS) {
    sleep( 1);
    c.s = k;
    k += 1;
  }
}
