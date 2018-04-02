
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
