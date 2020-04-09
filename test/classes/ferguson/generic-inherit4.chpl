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
  type u;
  var y:u;
  override proc overridden_method() {
    writeln(x,y);
  }
  proc child_method() {
    writeln(y);
  }
}

writeln("Parent(int)");
var p = new unmanaged Parent(int, 1);
p.parent_method();
p.overridden_method();
delete p;

writeln("Child(int,real)");
var c = new unmanaged Child(int, 1, real, 2.0);
c.parent_method();
c.overridden_method();
c.child_method();

writeln("Dynamic Child(int,real)");
var pc:borrowed Parent(int) = c;
pc.parent_method();
pc.overridden_method();

var upc:unmanaged Parent(int) = c;
delete upc;
