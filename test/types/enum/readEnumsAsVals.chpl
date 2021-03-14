use IO;

proc readit(ref x) {
  writeln(x);

  readf("%t\n", x);
  writeln(x);
  readf("%s\n", x);
  writeln(x);
  readf("%i\n", x);
  writeln(x);
  readf("%u\n", x);
  writeln(x);
  readf("%xi\n", x);
  writeln(x);
  readf("%r\n", x);
  writeln(x);
  readf("%er\n", x);
  writeln(x);
  readf("%n\n", x);
  writeln(x);

  writeln();
}

enum F { F1 = 4 };
var f = F.F1;
readit(f);

enum G { F1, F2 = 5};
var g = G.F2;
readit(g);

