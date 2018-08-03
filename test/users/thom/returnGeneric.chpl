class C {
  var n;
}

proc foo(): unmanaged C {
  return new unmanaged C(3);
}

writeln(foo());
