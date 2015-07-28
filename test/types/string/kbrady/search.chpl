proc test(hay, needle) {
  writeln("count: ", hay.count(needle));
  writeln("find: ", hay.find(needle));
  writeln();
}
proc test(hay, needle, r) {
  writeln("count: ", hay.count(needle, r));
  writeln("find: ", hay.find(needle, r));
  writeln();
}

var x = "Hello, hello, hello";
var c = "cccccccccccccccccccccccccccc";
var empty = "";

writeln("x: ", x);
writeln("c: ", c, "\n");

writeln('x.___("Hello")');
test(x, "Hello");

writeln('x.___("Hello", 2..)');
test(x, "Hello", 2..);

writeln('x.___("hello")');
test(x, "hello");

writeln('x.___("hello", 14..)');
test(x, "hello", 14..);

writeln('c.___("c")');
test(c, "c");

writeln('c.___("c", ..9)');
test(c, "c", ..9);

writeln('c.___("C")');
test(c, "C");

writeln('c.___("C", 5..9)');
test(c, "C", 5..9);

writeln('x.___("")');
test(x, empty);

writeln('x.___("", 9..15)');
test(x, empty, 9..15);

writeln('"".___(x)');
test(empty, x);

writeln('"".___("")');
test(empty, empty);


