proc f(out a: [] int, out b: [] int) {
  var tmp: [1..4] int;
  a = tmp;
  b = tmp;

  a = 1;
  b = 2;
}

var a: [1..4] int = 2, b: [1..4] int = 1;
f(a=a, b=b);
f(b=b, a=a);
writeln((a, b));
