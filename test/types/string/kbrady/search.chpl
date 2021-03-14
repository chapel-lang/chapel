proc test(hay, needle) {
  writeln("count: ", hay.count(needle));
  writeln("find: ", hay.find(needle));
  writeln("rfind: ", hay.rfind(needle));
  writeln();
}
proc test(hay, needle, r) {
  writeln("count: ", hay.count(needle, r));
  writeln("find: ", hay.find(needle, r));
  writeln("rfind: ", hay.rfind(needle, r));
  writeln();
}

var x = "Hello, hello, hello";
var y = "Allô, allô, allô";
var c = "cccccccccccccccccccccccccccc";
var d = "çççççççççççççççççççççççççççç";
var empty = "";

writeln("x: ", x);
writeln("y: ", y);
writeln("c: ", c);
writeln("d: ", d, "\n");

writeln('x.___("Hello")');
test(x, "Hello");

writeln('x.___("Hello", 1..)');
test(x, "Hello", 1..);

writeln('x.___("hello")');
test(x, "hello");

writeln('x.___("hello", 13..)');
test(x, "hello", 13..);

writeln('y.___("Allô")');
test(y, "Allô");

writeln('y.___("Allô", 1..)');
test(y, "Allô", 1..);

writeln('y.___("allô")');
test(y, "allô");

writeln('y.___("allô", 11..)');
test(y, "allô", 11..);

writeln('c.___("c")');
test(c, "c");

writeln('c.___("c", ..8)');
test(c, "c", ..8);

writeln('c.___("C")');
test(c, "C");

writeln('c.___("C", 4..8)');
test(c, "C", 4..8);

writeln('d.___("ç")');
test(d, "ç");

writeln('d.___("ç", ..17)');
test(d, "ç", ..17);

writeln('d.___("Ç")');
test(d, "Ç");

writeln('d.___("Ç", 8..17)');
test(d, "Ç", 8..17);

writeln('x.___("")');
test(x, empty);

writeln('x.___("", 8..14)');
test(x, empty, 8..14);

writeln('"".___(x)');
test(empty, x);

writeln('y.___("")');
test(y, empty);

writeln('y.___("", 8..14)');
test(y, empty, 8..14);

writeln('"".___(y)');
test(empty, y);

writeln('"".___("")');
test(empty, empty);
