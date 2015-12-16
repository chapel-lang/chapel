var csv = "1,'abc foo',1.00,$123,,baz";
var fox = "The quick brown fox jumps over the lazy dog";
var wide = "1 | 2 | ";

proc test(s, sep) {
  writeln(s, ":");
  for x in s.split(sep) {
    write('"', x, '"', " ");
  }
  writeln();
  var A = s.split(sep, ignoreEmpty=true);
  for a in A {
    write('"', a, '"', " ");
  }
  writeln();
  for x in s.split(sep, maxsplit=4) {
    write('"', x, '"', " ");
  }
  writeln();

  for x in s.split(sep, maxsplit=1) {
    write('"', x, '"', " ");
  }
  writeln();
}

test(csv, ',');
test(fox, ' ');
test(wide, ' | ');
