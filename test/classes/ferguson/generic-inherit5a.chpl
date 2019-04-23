class Parent {
  type t;
  var x:t;
  proc parent_method() {
    writeln(x);
  }
  proc overridden_method() {
    writeln(x);
  }

}

class Child : Parent {
  type t;
  var y:t;
  override proc overridden_method() {
    writeln(x,y);
  }
  proc child_method() {
    writeln(y);
  }
}

writeln("Parent(int)");
var p = new owned Parent(int, 1);
p.parent_method();
p.overridden_method();

writeln("Child(int,real)");
var c = new owned Child(int, 1, real, 2.0);
c.parent_method();
c.overridden_method();
c.child_method();

writeln("Dynamic Child(int,real)");
var pc:Parent(int) = c.borrow();
pc.parent_method();
pc.overridden_method();
