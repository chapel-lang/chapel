class Grandparent {}

class Parent : Grandparent {
  var x: int;
}

class Child1 : Parent {
  var y: int;
}

class Child2 : Parent {
  var z: int;
}

class Unrelated {
  var w: int;
}

proc foo(x: bool) {
  if x {
    return new Child1();
  } else {
    return new Parent();
  }
}

var a = foo(true);
writeln(a.type : string);
writeln(a);
