class Parent {
  var covered: int;

  proc init() {
    covered = 4;
  }
}

class Child: Parent {
  var covered: int;

  proc init(val) {
    covered = val;
  }
}

var child = new Child(10);
writeln(child);
delete child;
