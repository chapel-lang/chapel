use Vector;

config const testIters = 4;

record innerVector {
  var lst = new vector(int);

  proc deinit() {
    writeln(">> Destroying inner vector...");
    writeln(lst);
  }
}

record outerVector {
  var lst = new vector(innerVector);

  proc deinit() {
    writeln(">> Destroying outer vector...");
    writeln(lst);
  }
}

var outer = new outerVector();

var inner = new innerVector();
inner.lst.append(0);

for i in 1..testIters {
  outer.lst.append(inner);
}


