proc foo(param size: int) type return int(size);

var i16: foo(16);
writeln((i16,min(i16.type),max(i16.type)));
var i32: foo(32);
writeln((i32,min(i32.type),max(i32.type)));
var i64: foo(64);
writeln((i64,min(i64.type),max(i64.type)));
