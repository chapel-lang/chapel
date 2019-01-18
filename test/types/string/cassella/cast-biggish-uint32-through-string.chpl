/* 32bit LONG_MAX + LONG_MAX/2 + 1234. */
var biggish:uint(32) = 1234:uint(32) + (3:uint(32)<<30);
writeln(biggish, " == ", biggish:string:uint(32));
