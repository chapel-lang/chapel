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

writeln('x.___("Hello", 2..)');
test(x, "Hello", 2..);

writeln('x.___("hello")');
test(x, "hello");

writeln('x.___("hello", 14..)');
test(x, "hello", 14..);

writeln('y.___("Allô")');
test(y, "Allô");

writeln('y.___("Allô", 2..)');
test(y, "Allô", 2..);

writeln('y.___("allô")');
test(y, "allô");

writeln('y.___("allô", 14..)');
test(y, "allô", 12..);

writeln('c.___("c")');
test(c, "c");

writeln('c.___("c", ..9)');
test(c, "c", ..9);

writeln('c.___("C")');
test(c, "C");

writeln('c.___("C", 5..9)');
test(c, "C", 5..9);

writeln('d.___("ç")');
test(d, "ç");

writeln('d.___("ç", ..18)');
test(d, "ç", ..18);

writeln('d.___("Ç")');
test(d, "Ç");

writeln('d.___("Ç", 9..18)');
test(d, "Ç", 9..18);

writeln('x.___("")');
test(x, empty);

writeln('x.___("", 9..15)');
test(x, empty, 9..15);

writeln('"".___(x)');
test(empty, x);

writeln('y.___("")');
test(y, empty);

writeln('y.___("", 9..15)');
test(y, empty, 9..15);

writeln('"".___(y)');
test(empty, y);

writeln('"".___("")');
test(empty, empty);
