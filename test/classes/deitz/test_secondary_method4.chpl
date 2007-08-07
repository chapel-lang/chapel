class C {
  var y: int;
}

def foo() {
  def C.a() {
    writeln(y);
  }
  var a = C();
  a.a();
}

foo();
