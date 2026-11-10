// Check that a blocking GET split into chunks doesn't return before every
// chunk has arrived.
config const iters = 10000;
config const nElems = 512;

var A: [0..#nElems] int = 0..#nElems;

on Locales[numLocales - 1] {
  var B: [0..#nElems] int;
  var stale = 0;
  for 1..iters {
    B = -1;
    B = A;
    for i in 0..#nElems {
      if B[i] != i {
        stale += 1;
        break;
      }
    }
  }
  writeln("stale GETs: ", stale);
}
