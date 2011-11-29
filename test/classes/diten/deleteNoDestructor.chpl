class C {
  var a,b: int;
}

proc main {
  var c = new C(1,2);
  writeln(c);
  delete c;
}
