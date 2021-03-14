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
  var y:t;
  override proc overridden_method() {
    writeln(x,y);
  }
  proc child_method() {
    writeln(y);
  }
}
writeln("Dynamic Child(int)");
var pc:borrowed Parent(int) = new borrowed Child(int, 1, 2);
pc.parent_method();
pc.overridden_method();
