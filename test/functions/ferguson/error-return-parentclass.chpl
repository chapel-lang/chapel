class Parent {
  var x:int;
}

class Child : Parent {
  var y:int;
}

var globalParent = new borrowed Parent(1);
var got = f();
writeln(got);


proc f() : borrowed Child {
  return globalParent;
}


