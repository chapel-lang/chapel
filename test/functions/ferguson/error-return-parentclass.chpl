class Parent {
  var x:int;
}

class Child : Parent {
  var y:int;
}

var globalParent = new Parent(1);
var got = f();
writeln(got);
delete globalParent;

proc f() : Child {
  return globalParent;
}


