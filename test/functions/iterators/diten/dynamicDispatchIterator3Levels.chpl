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
  override iter myit() {
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
  override iter myit() {
    var low = 2;
    var high = 8;
    var stride = 2;

    writeln("Grandchild");
    for i in low..high by stride {
      yield i;
    }
  }
}

var ownP1 = new owned Child();
var p1: borrowed Parent = ownP1.borrow();
var ownP2 = new owned Grandchild();
var p2: borrowed Parent = ownP2.borrow();

for i in p1.myit() {
  writeln(i);
}

for i in p2.myit() {
  writeln(i);
}
