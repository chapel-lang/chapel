inline proc _defaultOf(type t) param where (_isBooleanType(t)) return true:t;

var foo: bool;
writeln(foo);
