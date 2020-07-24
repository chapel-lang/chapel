use OrderedMap;

config param impl: mapImpl;

{
  var m = new orderedMap(string, int, false, defaultComparator, impl);
  m["one"] = 1;
  writeln(m);
}

{
  record R {
    var a: int;
  }
  var m = new orderedMap(R, real);
  m[new R(1)] = 1.1;

  var m2 = m;
  writeln(m2);
}

{
  var m= new orderedMap(int, string);
  m[1] = "hello";
  m[4] = "world";

  writeln(m.toArray().sorted());
}
