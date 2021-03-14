inline proc _defaultOf(type t) param where (isIntType(t)) return 13:t;

var foo: [0:uint(8)..255:uint(8)] int;

writeln(foo[42:uint(8)]);
