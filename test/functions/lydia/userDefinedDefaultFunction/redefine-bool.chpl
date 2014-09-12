inline proc _defaultOf(type t) param where (isBoolType(t)) return true:t;

var foo: bool;
writeln(foo);
