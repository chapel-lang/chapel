use IO;

proc print(x) {
  writeln(x);

  var s1 = "%s".format(x);
  writeln(s1);

  var s2 = "%t".format(x);
  writeln(s2);
}

enum E { E1 };
var e = E.E1;
print(e);

enum F { F1 = 4 };
var f = F.F1;
print(f);
