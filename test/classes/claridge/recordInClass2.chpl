record R {
  var indices = {1..0};
}

class Container {
  var r = new R();
}

proc main {
  var c = new Container();
  writeln("12345678901234567");
  c.r.indices = {1..1};
}
