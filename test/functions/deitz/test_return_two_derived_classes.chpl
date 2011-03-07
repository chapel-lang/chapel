class C {
  var x: int;
}

class D: C {
  var y: int;
}

class E: C {
  var z: int;
}

proc f(b: bool) {
  if b then
    return new D();
  else
    return new E();
}

writeln(f(false), f(true));
