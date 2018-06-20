class Parent {
  proc f(x:int) {
    writeln("Parent.f(int)");
  }
}

class Child : Parent {
  // is a generic override of concrete method
  proc f(x) {
    writeln("Child.f(generic)");
  }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
  var z = y:borrowed Parent;
  y.f(1);
  // Currently an ambiguity error but the important
  // thing is that it's *some* sort of compilation failure.
}
test();
