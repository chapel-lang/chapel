use Maps;

{
  var m = new map(string, int);
  m["one"] = 1;
  writeln(m);
}

{
  record R {
    var a: int;
  }
  var m = new map(R, real);
  m[new R(1)] = 1.1;

  var m2 = m;
  writeln(m2);
}
