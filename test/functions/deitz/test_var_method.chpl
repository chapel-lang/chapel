class C {
  var i : int;
  proc foo() ref {
    return i;
  }
}

var c : C = new C();
c.i = 2;
writeln(c);
writeln(c.foo());
c.foo() = 3;
writeln(c);
delete c;
