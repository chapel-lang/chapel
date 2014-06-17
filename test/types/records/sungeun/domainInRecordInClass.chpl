record myR {
  var d = {1..0};
}

class myC {
  var r = new myR();
}

proc main {
  var c = new myC();
  writeln(c);
  c.r.d = {1..1};
  writeln(c);
}
