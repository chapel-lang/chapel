proc f(out a: int, out b: int) {
  a = 1;
  b = 2;
}

var a = 2, b = 1;
f(b=b, a=a);
writeln((a, b));
