class Parent {
  type u;
  var x:u;
  proc parent_method() {
    writeln(x);
  }
  proc overridden_method() {
    writeln(x);
  }

}

class Child : Parent(int) {
  type t;
  var y:t;
  proc overridden_method() {
    writeln(x,y);
  }
  proc child_method() {
    writeln(y);
  }
}

writeln("Parent(int)");
var p = new Parent(int, 1);
p.parent_method();
p.overridden_method();

writeln("Child(int)");
var c = new Child(1, int, 2);
c.parent_method();
c.overridden_method();
c.child_method();

writeln("Dynamic Child(int)");
var pc:Parent = c.borrow();
pc.parent_method();
pc.overridden_method();

