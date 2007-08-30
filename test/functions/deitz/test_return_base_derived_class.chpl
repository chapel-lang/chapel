class C {
  var x: int;
}

class D: C {
  var y: int;
}

def f(b: bool) {
  if b then
    return C();
  else
    return D();
}

writeln(f(false), f(true));
