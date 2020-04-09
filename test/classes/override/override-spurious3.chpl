class Parent {
  proc f() {
    writeln("in Parent.f()");
  }
  proc g(arg) {
    writeln("in Parent.g()");
  }

}

class Child : Parent {
  override proc f(default=0) {
    writeln("in Child.f(default)");
  }
  override proc g(arg, optionA="x") {
    writeln("in Child.g(optionA)");
  }
  override proc g(arg, optionB="x") {
    writeln("in Child.g(optionB)");
  }

}

var c = new Child();
//c.f(1); error: ambiguous call

var p = c:Parent;
p.f();
p.g(1);
