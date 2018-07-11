pragma "use default init"
record bar {
  type t;
  var aField: t;
}

inline proc _defaultOf(type t:bar(int(8))) {
  var res: t = noinit;
  res.aField = -1;
  return res;
}

var foo: bar(int(32));
var baz: bar(int(8));
writeln(foo);
writeln(baz);
