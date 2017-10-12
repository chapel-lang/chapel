class Parent {
  iter myit() {
    var low = 1;
    var high = 4;
    writeln("Parent");
    for i in low..high {
      yield i;
    }
  }
}

class Child: Parent {
  iter myit() {
    var low = 1;
    var high = 8;
    var stride = 2;
    writeln("Child");
    for i in low..high by stride {
      yield i;
    }
  }
}

class Grandchild: Child {
  iter myit() {
    var low = 2;
    var high = 8;
    var stride = 2;

    writeln("Grandchild");
    for i in low..high by stride {
      yield i;
    }
  }
}

var p1: Parent = new Child();
var p2: Parent = new Grandchild();

for i in p1.myit() {
  writeln(i);
}

for i in p2.myit() {
  writeln(i);
}
