class C {
  var y: int;
}

proc foo() {
  proc C.a() {
    writeln(y);
  }
  var a = new C();
  a.a();
  delete a;
}

foo();
