var s = b"123456789abcdef";
var sv = s[1..10 by 2 align 2 ];
writeln(sv);

var sv2 = s[-1.. by 3 align 0];
writeln(sv2);
