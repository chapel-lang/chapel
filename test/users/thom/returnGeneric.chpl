class C {
  var n;
}

def foo(): C {
  return new C(3);
}

writeln(foo());
