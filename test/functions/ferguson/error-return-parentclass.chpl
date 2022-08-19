class Parent {
  var x:int;
}

class Child : Parent {
  var y:int;
}

var globalParent = (new owned Parent(1)).borrow();
var got = f();
writeln(got);


proc f() : borrowed Child {
  return globalParent;
}


