proc foo(o: RootClass) {
  writeln("I'm in foo()");
}

var mysync: sync int;

foo(mysync);
