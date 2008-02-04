class C {
  var y: int;
}

def foo() {
  def C.a() {
    writeln(y);
  }
  var a = new C();
  a.a();
}

foo();
