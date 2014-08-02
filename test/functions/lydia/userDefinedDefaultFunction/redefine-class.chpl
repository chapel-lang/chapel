class bar {
  var aField: int;
}

proc _defaultOf(type t):t where t:bar {
  var res: t = noinit;
  res = new bar(4);
  return res;
}

var foo: bar;
writeln(foo);
