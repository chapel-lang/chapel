record myR {
  var s: string;
}

proc main {
  var r: myR;
  var s = "hi";
  writeln(r);
  r.s = s;
  writeln(r);
}
