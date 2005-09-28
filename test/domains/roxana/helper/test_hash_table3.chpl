var s : integer = 10;
--writeln("Enter table size");
--read(s);
var table : _fdata(integer) = _fdata(integer, s);

for i in 1..s {
  if (table(i-1) == 0) {
    table(i-1) = i;
  }
  writeln(table(i-1));
}
