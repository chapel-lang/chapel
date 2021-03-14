record foo {
  var a: int;
  var b: int;
}

inline proc _defaultOf(type t) where t:foo {
  var result:t = noinit;
  result.a = 3;
  result.b = _defaultOf(result.b.type);
  return result;
}

var bar: foo;
writeln(bar);
