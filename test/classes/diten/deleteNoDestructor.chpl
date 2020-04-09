class C {
  var a,b: int;
}

proc main {
  var c = new unmanaged C(1,2);
  writeln(c);
  delete c;
}
