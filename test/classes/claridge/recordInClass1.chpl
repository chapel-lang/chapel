record R {
  var indices = {1..0};
}

class Container {
  var r = new R();
}

proc main {
  var c = new Container();
  // writeln(c.r.indices);
  writeln("hello");
  c.r.indices = {1..1};
}
