record myR {
  var d = {1..0};
}

proc main {
  var r: myR;
  writeln(r);
  r.d = {1..1};
  writeln(r);
}
