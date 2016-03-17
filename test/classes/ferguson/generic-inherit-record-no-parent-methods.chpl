record Parent {
  type t;
  var x:t;
  proc parent_method() {
    writeln(x);
  }
  proc overridden_method() {
    writeln(x);
  }

}

record Child : Parent {
  var y:t;
  proc overridden_method() {
    writeln(x,y);
  }
  proc child_method() {
    writeln(y);
  }
}

// OK
writeln("Parent(int)");
var p = new Parent(int, 1);
p.parent_method();
p.overridden_method();

writeln("Child(int)");
var c = new Child(int, 1, 2);
c.overridden_method();
c.child_method();

