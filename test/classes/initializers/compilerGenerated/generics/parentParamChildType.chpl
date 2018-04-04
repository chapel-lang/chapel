
class Dummy {
  param stridable : bool;
}

class Parent {
  param stridable : bool;
  var x : int;
}

class Child : Parent {
  var y : Dummy(stridable);
}

var c = new Child(false, 5);
writeln('c = ', c);
delete c;

class A {
  param rank : int;
  type idxType;
  param stridable : bool;
}

class Z : A {
  var x : Dummy(stridable);
}

var z = new Z(1, int, false);
writeln("z = ", z);
