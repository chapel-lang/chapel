use Time;

// Multi-threaded test with a record that has a sync var field.

// Similar to record1.chpl, but instead fast writer, slow reader.
// Outputs what was read.

param ITERATIONS = 10;

record R {
  var s: sync int;
}
var c: R;

sync {
  writeln( "\ntest 2");
  begin with (ref c){
    var r: int;
    var j: int;
    j = 0;
    while (j < ITERATIONS) {
      sleep( 1);
      r = c.s.readFE();
      writeln( "2: got ", r);
      j += 1;
    }
  }

  var k: int;
  k = 0;
  while (k < ITERATIONS) {
    c.s.writeEF(k);
    k += 1;
  }
}
