class bar {
  var aField: int;
}

var global = new unmanaged bar(4);

// it is not currently possible to change the default
// value for a class type to be anything other than nil.

var foo: unmanaged bar?;

writeln(foo);

delete global;

