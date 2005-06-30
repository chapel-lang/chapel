var s : integer = 10;
--writeln("Enter table size");
--read(s);
var table : _data(integer, s);

for i in 1..s {
  table(i) = i;
  writeln(table(i));
}
