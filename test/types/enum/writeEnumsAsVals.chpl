proc print(x) {
  writeln(x);

  writef("%s\n", x);
  writef("%t\n", x);
  writef("%i\n", x);
  writef("%u\n", x);
  writef("%xi\n", x);
  writef("%r\n", x);
  writef("%er\n", x);
  writef("%n\n", x);

  writeln();
}

enum F { F1 = 4 };
var f = F.F1;
print(f);

enum G { F1, F2 = 5};
var g = G.F2;
print(g);

