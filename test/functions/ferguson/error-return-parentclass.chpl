class Parent {
  var x:int;
}

class Child : Parent {
  var y:int;
}
var globalParentObj = new Parent(1);
var globalParent = globalParentObj.borrow();
var got = f();
writeln(got);


proc f() : borrowed Child {
  return globalParent;
}
