inline proc _defaultOf(type t) param where (_isIntegralType(t)) return 5:t;

var foo:int;
writeln(foo);
