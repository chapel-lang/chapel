class Grandparent {
  var x: int;
}

class Parent : Grandparent {
  var z: int;
}

class Child : Parent {
  var x: int;
}

var obj = new Child();
writeln(obj.x);
