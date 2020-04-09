proc f(x:enum) {
  writeln(x);
}

enum e {a=1};

var a:sync e = e.a;
f(a);

proc g(x:integral) {
  writeln(x);
}

var b:single int = 1;
g(b);
