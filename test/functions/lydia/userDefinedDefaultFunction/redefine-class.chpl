class bar {
  var aField: int;
}

var global = new unmanaged bar(4);

proc _defaultOf(type t):t where t:unmanaged bar {
  return global:unmanaged bar;
}

var foo: unmanaged bar;

writeln(foo);

delete foo;

