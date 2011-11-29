// wantInt64.chpl
//
// Let int = int(64) and uint = uint(64).
//
var i : int = 0;
var u :uint = 1;

writeln(typeToString(i.type));
writeln(typeToString(u.type));

writeln(typeToString((i+1).type));
writeln(typeToString((u+1).type));

var i8:int(8) = -8;
var i16:int(16) = -16;
var i32:int(32) = -32;
var i64:int(64) = -64;
var ui8:uint(8) = 8;
var ui16:uint(16) = 16;
var ui32:uint(32) = 32;
var ui64:uint(64) = 64;

writeln(typeToString((i+i8).type));
writeln(typeToString((i+i16).type));
writeln(typeToString((i+i32).type));
writeln(typeToString((i+i64).type));
writeln(typeToString((i+ui8).type));
writeln(typeToString((i+ui16).type));
writeln(typeToString((i+ui32).type));
// Another future: Want this to be legal, but generate a run-time error if result does not fit.
// This means we must specify a winner at compile time: int or uint.
// writeln(typeToString((i+ui64).type));
writeln(typeToString((u+i8).type));
writeln(typeToString((u+i16).type));
writeln(typeToString((u+i32).type));
writeln(typeToString((u+i64).type));
writeln(typeToString((u+ui8).type));
writeln(typeToString((u+ui16).type));
writeln(typeToString((u+ui32).type));
// This is a bug: Compiler should accept this.
// writeln(typeToString((u+ui64).type));

writeln(typeToString((1+i8).type));
writeln(typeToString((1+i16).type));
writeln(typeToString((1+i32).type));
writeln(typeToString((1+i64).type));
writeln(typeToString((1+ui8).type));
writeln(typeToString((1+ui16).type));
writeln(typeToString((1+ui32).type));
writeln(typeToString((1+ui64).type));


