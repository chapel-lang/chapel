record Parent {
  var x:int;
  proc parent_method() {
    writeln(x);
  }
  proc overridden_method() {
    writeln(x);
  }

}

record Child : Parent {
  type t;
  var y:t;
  proc overridden_method() {
    writeln(x,y);
  }
  proc child_method() {
    writeln(y);
  }
}

writeln("Parent");
var p = new Parent(1);
p.parent_method();
p.overridden_method();

writeln("Child(int)");
var c = new Child(1, int, 2);
c.parent_method();
c.overridden_method();
c.child_method();

