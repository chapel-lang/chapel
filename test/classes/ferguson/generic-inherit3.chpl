class Parent {
  var x:int;
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

writeln("Parent");
var p = new unmanaged Parent(1);
p.parent_method();
p.overridden_method();
delete p;

writeln("Child(int)");
var c = new unmanaged Child(1, int, 2);
c.parent_method();
c.overridden_method();
c.child_method();

writeln("Dynamic Child(int)");
var pc:borrowed Parent = c;
pc.parent_method();
pc.overridden_method();

var upc:unmanaged Parent = c;
delete upc;
