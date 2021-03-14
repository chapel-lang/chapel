record R {
  param foo: int(64);

}

type t = R(5);
var paramField = __primitive("query param field", t, "foo");
writeln(paramField);
