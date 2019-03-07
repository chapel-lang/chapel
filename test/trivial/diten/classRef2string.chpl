class C {
  var a, b:int;
}

proc main {
  var a: int;
  var b = new borrowed C(1, 2);
  writeln(chpldev_refToString(a));
  writeln(chpldev_refToString(b));
  writeln(chpldev_refToString(b.a));
  writeln(chpldev_refToString(b.b));
}
