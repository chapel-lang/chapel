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

var sharedOneToTen = new borrowed OneToTen();

cobegin {
  for x in sharedOneToTen.count() do register(1, x);
  for y in sharedOneToTen.count() do register(2, y);
}

proc register(taskid, i) {
  A(i) = taskid;
}


for i in D {
  write("A(", i, ") registered");
  if printTaskID then write(" by task ", A(i));
  writeln();
}
