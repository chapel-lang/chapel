proc foo(o: object) {
  writeln("I'm in foo()");
}

var mysync: sync int;

foo(mysync);
