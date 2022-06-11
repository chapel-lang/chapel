// Test for issue #19958

var zero:int = 0;
var u8:uint(8) = 2;
var u16:uint(16) = 2;
var u32:uint(32) = 2;
var u64:uint(64) = 2;

writeln("iterating over 0..#u8");
for x in 0..#u8 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over zero..#u8");
for x in zero..#u8 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over 0..#u16");
for x in 0..#u16 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over zero..#u16");
for x in zero..#u16 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over 0..#u32");
for x in 0..#u32 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over zero..#u32");
for x in zero..#u32 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over 0..#u64");
for x in 0..#u64 {
  writeln(x, ":", x.type:string);
}

writeln("iterating over zero..#u64");
for x in zero..#u64 {
  writeln(x, ":", x.type:string);
}
