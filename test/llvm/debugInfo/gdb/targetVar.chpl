const myGlobal = 42;

proc foo(myFormal) {
  const localVar = myFormal + 1;
  writeln("printing from foo");
  // CHECK: hit Breakpoint 2
  // CHECK: at ./targetVar.chpl:5
  // CHECK: myFormal = 59
  // CHECK: localVar = 60
  writeln("myFormal: ", myFormal);
  writeln("localVar: ", localVar);
}

proc bar(myFormal) {
  // CHECK: hit Breakpoint 3
  // CHECK: at ./targetVar.chpl:14
  // CHECK: myFormal = 59
  // CHECK: No locals.
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
