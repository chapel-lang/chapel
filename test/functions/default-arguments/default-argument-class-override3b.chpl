proc f() type { return int; }
proc g() type { return real; }

class Parent {
  proc method(type arg = f()) {
    writeln("in Parent.method arg=", arg:string);
  }
}

class Child : Parent {
  override proc method(type arg = g()) {
    writeln("in Child.method arg=", arg:string);
  }
}

proc main() {
  var x:Parent = new Child();
  x.method();

  var y = new Child();
  y.method();

  var z = new Parent();
  z.method();
}
