var s : integer = 10;
--writeln("Enter table size");
--read(s);
var table : _data(integer, s);
table.alloc(s, "table");
for i in 1..s {
  table(i-1) = i;
  writeln(table(i-1));
}
