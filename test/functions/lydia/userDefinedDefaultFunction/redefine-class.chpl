class bar {
  var aField: int;
}

var global = new unmanaged bar(4);

proc _defaultOf(type t):t where t:bar {
  return global:bar;
}

var foo: bar;

writeln(foo);

delete foo;

