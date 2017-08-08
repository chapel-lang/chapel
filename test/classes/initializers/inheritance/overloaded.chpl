class Parent {
  var covered: int;

  proc init() {
    covered = 4;
    super.init();
  }
}

class Child: Parent {
  var covered: int;

  proc init(val) {
    covered = val;
    super.init();
  }
}

var child = new Child(10);
writeln(child);
delete child;
