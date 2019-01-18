class C {
  var x: [1..3] int;
  proc foo() ref {
    writeln(true);
    return x;
  }
  proc foo() {
    writeln(false);
    return x;
  }

}

var c = new unmanaged C();

c.foo() = 2;
writeln(c.foo());
delete c;
