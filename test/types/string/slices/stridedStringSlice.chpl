var s = "123456789abcdef";
var sv = s[1:byteIndex..10:byteIndex by 2 align 2:byteIndex ];
writeln(sv);

var sv2 = s[(-1):byteIndex.. by 3 align 0:byteIndex];
writeln(sv2);
