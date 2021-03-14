record myR {
  var a: [1..2] int;
}

class myC {
  var r = new myR();
}

proc main {
  var c             = new unmanaged myC();
  var a: [2..3] int = -1;

  writeln(c);
  c.r.a = a;
  writeln(c);

  delete c;
}
