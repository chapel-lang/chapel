proc f() {
  return 0;
}

proc g() {
  return 1;
}

class Parent {
  proc method(arg:int = f()) {
    writeln("in Parent.method arg=", arg);
  }
}

class Child : Parent {
  override proc method(arg:int = g()) {
    writeln("in Child.method arg=", arg);
  }
}

proc main() {
  var x:Parent = new Child();
  x.method();

  var y = new Child();
  y.method();
}
