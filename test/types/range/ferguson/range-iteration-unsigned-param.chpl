param u8:uint(8) = 2;
writeln("iterating over 0..u8");
for x in 0..u8 {
  writeln(x, ":", x.type:string);
}

param u16:uint(16) = 2;
writeln("iterating over 0..u16");
for x in 0..u16 {
  writeln(x, ":", x.type:string);
}

param u32:uint(32) = 2;
writeln("iterating over 0..u32");
for x in 0..u32 {
  writeln(x, ":", x.type:string);
}

param u64:uint(64) = 2;
writeln("iterating over 0..u64");
for x in 0..u64 {
  writeln(x, ":", x.type:string);
}
