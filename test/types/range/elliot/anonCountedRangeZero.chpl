for i in 0..#0 {
  writeln("This should not be printed");
}
for i in 1..#0 {
  writeln("This should not be printed");
}
for i in 17..#0 {
  writeln("This should not be printed");
}
const r1 = 0..#0;
writeln("r1: ", r1);
const r2 = 1..#0;
writeln("r2: ", r2);
const r3 = 17..#0;
writeln("r3: ", r3);
