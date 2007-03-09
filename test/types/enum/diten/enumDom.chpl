enum color { red, green, blue };

var D: domain(real);
var E: domain(color);

D.add(1.1);
D.add(2.2);
D.add(3.3);

E.add(red);   // Shouldn't need to do this.
E.add(green); // Shouldn't need to do this.
E.add(blue);  // Shouldn't need to do this.

var s = (/ 3, 4, 5 /);
var a: [D] int = s;
var b: [E] int = s;

writeln("(a.domain, s)");
for (i, j) in (a.domain, s) {
  writeln(i, " ", j);
}
writeln("(s, a.domain)");
for (j, i) in (s, a.domain) {
  writeln(i, " ", j);
}
writeln("(b.domain, s)");
for (i, j) in (b.domain, s) {
  writeln(i, " ", j);
}
writeln("(s, b.domain)");
for (j, i) in (s, b.domain) {
  writeln(i, " ", j);
}
