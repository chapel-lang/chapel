proc foo(inout x: string) {
  x += " attached";
}

var s = "hello world";
foo(s);
writeln(s);

var i = 12;
foo(i);
writeln(i);
