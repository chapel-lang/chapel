record bar {
  param size: int;
  var whatev = 5;
}

proc _defaultOf(type t:bar(3)) {
  var res: bar(3) = noinit;
  res.whatev = 7;
  return res;
}

var foo: bar(3);
var baz: bar(2);
writeln("(size = ", foo.size, ", whatev = ", foo.whatev, ")");
writeln("(size = ", baz.size, ", whatev = ", baz.whatev, ")");
