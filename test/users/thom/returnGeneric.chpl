class C {
  var n;
}

proc foo(): C {
  return new C(3);
}

writeln(foo());
