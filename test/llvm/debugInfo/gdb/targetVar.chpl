const myGlobal = 42;

proc foo(myFormal) {
  const localVar = myFormal + 1;
  writeln("printing from foo");
  writeln("myFormal: ", myFormal);
  writeln("localVar: ", localVar);
}

proc bar(myFormal) {
  writeln("printing from bar");
  writeln("myFormal: ", myFormal);
}

proc main() {
  var myLocal: int = 17 + myGlobal;
  writeln("myLocal: ", myLocal);
  writeln("myGlobal: ", myGlobal);
  foo(myLocal);
  bar(myLocal);
}
