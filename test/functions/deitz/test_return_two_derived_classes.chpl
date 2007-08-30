class C {
  var x: int;
}

class D: C {
  var y: int;
}

class E: C {
  var z: int;
}

def f(b: bool) {
  if b then
    return D();
  else
    return E();
}

writeln(f(false), f(true));
