record R {
  param foo: int(64);

}

type t = R(5);
var paramField = t.foo;
writeln(paramField);
