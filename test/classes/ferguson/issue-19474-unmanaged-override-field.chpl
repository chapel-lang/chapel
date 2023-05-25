class Parent {
  param rank : int;
}

class Child : Parent {
  override proc rank param do return 2;
}

var x = new unmanaged Child(1);
writeln(x.rank);
delete x;
