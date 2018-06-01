class Parent {
  proc f() {
    writeln("Parent.f()");
  }

  proc f_generic(x) {
    writeln("Parent.f_generic()");
  }
}

// TODO -- test names differ, types the same
//         test types differ, arg names the same
//         test generic vs concrete

class Child : Parent {
  override proc g() {
    writeln("Child.g()");
  }

  /*override*/ proc f() {
    writeln("Child.f()");
  }

  override proc g_generic(x) {
    writeln("Child.g_generic()");
  }

  /*override*/ proc f_generic(x) {
    writeln("Child.f_generic()");
  }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
  y.f();
  y.g();
  y.g_generic(1);
  y.f_generic(1);
}
test();
