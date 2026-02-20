use Math;

config const signed = true,
             size = 8;

proc testLCM(type t, x:t) {
  write("   ");
  for j in 1..x do
    writef("%3i ", j);
  writeln();

  write("---");
  for j in 1..x do
    write("----");
  writeln();

  for i in 1..x {
    writef("%2i: ", i);
    for j in 1..x {
      const res = lcm(i,j);
      writef(if signed then "%3i " else "%3u ", res);
    }
    writeln();
  }
}

if signed then
  select size {
    when 8 do testLCM(int(8), 20);
    when 16 do testLCM(int(16), 20);
    when 32 do testLCM(int(32), 20);
    when 64 do testLCM(int(64), 20);
  }
else
  select size {
    when 8 do testLCM(uint(8), 20);
    when 16 do testLCM(uint(16), 20);
    when 32 do testLCM(uint(32), 20);
    when 64 do testLCM(uint(64), 20);
  }


