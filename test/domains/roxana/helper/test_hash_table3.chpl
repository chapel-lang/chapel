var s : int = 10;
--writeln("Enter table size");
--read(s);
var table : _ddata(int) = _ddata(int, s);

table.init();

for i in 1..s {
  if (table(i-1) == 0) {
    table(i-1) = i;
  }
  writeln(table(i-1));
}
