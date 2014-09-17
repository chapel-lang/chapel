record bar {
  param size: int;
  var whatev = 5;
}

proc _defaultOf(type t) where t:bar(3) {
  var res: bar(3) = noinit;
  res.whatev = 7;
  return res;
}

var foo: bar(3);
var baz: bar(2);
writeln(foo);
writeln(baz);
