use LinkedLists;

enum color { red, green, blue };

var D: domain(real);
var E: domain(color) = {color.red, color.green, color.blue};

D.add(1.1);
D.add(2.2);
D.add(3.3);

var s          = makeList( 3, 4, 5 );

var a: [D] int = s;
var b: [E] int = s;

writeln("(a.domain, s)");

for (i, j) in zip(a.domain.sorted(), s) {
  writeln(i, " ", j);
}

writeln("(s, a.domain)");

for (j, i) in zip(s, a.domain.sorted()) {
  writeln(i, " ", j);
}

writeln("(b.domain, s)");

for (i, j) in zip(b.domain, s) {
  writeln(i, " ", j);
}

writeln("(s, b.domain)");

for (j, i) in zip(s, b.domain) {
  writeln(i, " ", j);
}

s.destroy();
