record myR {
  var a: [1..2] int;
}

proc main {
  var r: myR;
  var a: [2..3] int = -1;
  writeln(r);
  r.a = a;
  writeln(r);
}
