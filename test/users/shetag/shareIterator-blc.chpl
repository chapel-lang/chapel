config var numIters = 10;

config var printTaskID = true;

const D = {1..numIters};
var A: [D] int;

class OneToTen {
  var currIndex: sync int = 1;

  iter count() {
    var done = false;

    while (!done) {
      const myLocIndex = currIndex.readFE();  // read currIndex, leave empty;

      currIndex.writeEF(myLocIndex + 1);    // write currIndex, leave full;

      if myLocIndex <= numIters then
        yield myLocIndex;
      else
        done = true;
    }
  }
}

var sharedOneToTen = new unmanaged OneToTen();

cobegin {
  for x in sharedOneToTen.count() do A(x) = 1;
  for y in sharedOneToTen.count() do A(y) = 2;
}

for i in D {
  write("A(", i, ") registered");
  if printTaskID then write(" by task ", A(i));
  writeln();
}

delete sharedOneToTen;

