inline proc _defaultOf(type t) param where (isIntegralType(t)) return 5:t;

var foo:int;
writeln(foo);
