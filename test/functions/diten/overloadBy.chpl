proc by(a, b) where !isSubtype(a.type,range) {
  return a * b;
}

var a = (1,2,3) by (4,5,6);
writeln(a);
var b = 1..10 by 2;
writeln(b);
