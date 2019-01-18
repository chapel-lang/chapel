record R {
  var x:int;
  proc f() {
    writeln(x);
  }
  proc ref g() {
    writeln(x);
  }
  proc const h() {
    writeln(x);
  }
  proc const ref i() {
    writeln(x);
  }
}

var r = new R(1);

r.f();
r.g();
r.h();
r.i();
