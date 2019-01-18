record R {
  var indices = {1..0};
}

class Container {
  var r = new R();
}

proc main {
  var c = new unmanaged Container();

  // writeln(c.r.indices);
  writeln("hello");
  c.r.indices = {1..1};

  delete c;
}
