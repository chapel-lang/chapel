use List;

config const testIters = 4;

record innerList {
  var lst = new list(int);

  proc deinit() {
    writeln(">> Destroying inner list...");
    writeln(lst);
  }
}

record outerList {
  var lst = new list(innerList);

  proc deinit() {
    writeln(">> Destroying outer list...");
    writeln(lst);
  }
}

var outer = new outerList();

var inner = new innerList();
inner.lst.pushBack(0);

for i in 1..testIters {
  outer.lst.pushBack(inner);
}
