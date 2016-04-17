class C {
  var x;
  var y;
  proc foo() {
    return x + y;
  }
}

var c = new C(2, 5);
writeln(c.foo());

var d = new C("hello", "world");
writeln(d.foo());

delete c;
delete d;
