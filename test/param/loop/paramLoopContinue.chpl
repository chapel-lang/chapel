for param i in 1..10 {
  if i%2 == 0 then
    continue;
  writeln(i);
}

writeln("---");

for param i in 1..10 by -1 {
  if i%2 == 0 then
    continue;
  writeln(i);
}  

writeln("---");

for param i in 1..10 by 2{
  if i%4 == 3 then
    continue;
  writeln(i);
}

writeln("---");

for param i in 1..10 by -2 {
  if i%4 == 0 then
    continue;
  writeln(i);
}  

writeln("---");

