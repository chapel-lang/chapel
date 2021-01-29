class Parent {
  var x: int;

  operator <(lhs: Parent, rhs: Parent) {
    return lhs.x < rhs.x;
  }
}

class Child: Parent {
  var y: bool;
}

var c1 = new Parent(2);
var c2 = new Child(3, false);
writeln(c1 < c2);
writeln(c2 < c1);
var c3 = new Child(4, true);
writeln(c2 < c3);
writeln(c3 < c2);
