class C {
  var a, b:int;
}

def chpldev_classToString(ref: object) {
  return __primitive("ref to string", ref);
}

def main {
  var a: int;
  var b = new C(1, 2);
  writeln(chpldev_refToString(a));
  writeln(chpldev_refToString(b));
  writeln(chpldev_classToString(b));
  writeln(chpldev_refToString(b.a));
  writeln(chpldev_refToString(b.b));
}
