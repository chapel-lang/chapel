var i8: int(8);
var i16: int(16);
var i32: int(32);
var i64: int(64);

writeln((i8, i16, i32, i64));

i8 = 8;
i16 = i8;
i32 = i8;
i64 = i8;

writeln((i8, i16, i32, i64));

i16 = 16;
i32 = i16;
i64 = i16;

writeln((i8, i16, i32, i64));

i32 = 32;
i64 = i32;

writeln((i8, i16, i32, i64));

i64 = 1024;
i32 = i64:int(32);
i16 = i64:int(16);
i8 = i64:int(8);

writeln((i8, i16, i32, i64));
