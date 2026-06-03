record impl {
  proc doThing() {
    writeln("impl doing thing");
  }
}

record r1 {
  forwarding var implField: impl;
}

record r2 {
  var implField: impl;

  proc ref getImplField() ref {
    return implField;
  }

  forwarding getImplField();
}

var x1: r1;
x1.doThing();

var x2: r2;
x2.doThing();
