var a: atomic int;

proc foo(x) {
  x.add(1);
}

proc bar(x: atomic int) {
  x.add(1);
}

proc baz(ref x: atomic int) {
  x.add(1);
}

proc bax(ref x) {
  x.add(1);
}

writeln("a is: ", a.read());
foo(a);
writeln("a is: ", a.read());
bar(a);
writeln("a is: ", a.read());
baz(a);
writeln("a is: ", a.read());
bax(a);
writeln("a is: ", a.read());
