// CHECK: Stop hook #1
// CHECK-NEXT: Breakpoint [[#BREAKPOINT_START:]]

const myGlobal = 42;

proc foo(myFormal) {
  const localVar = myFormal + 1;
  writeln("printing from foo");
  // CHECK: stop reason = breakpoint [[#BREAKPOINT_START+1]]
  // CHECK-NEXT: targetVar.chpl:8
  // CHECK: target var
  // CHECK: (int(64)) myGlobal_chpl = 42
  // CHECK: frame var
  // CHECK: (int(64)) myFormal = 59
  // CHECK: (int(64)) localVar = 60
  writeln("myFormal: ", myFormal);
  writeln("localVar: ", localVar);
}

proc bar(myFormal) {
  // CHECK: stop reason = breakpoint [[#BREAKPOINT_START+2]]
  // CHECK-NEXT: targetVar.chpl:20
  // CHECK: target var
  // CHECK: (int(64)) myGlobal_chpl = 42
  // CHECK: frame var
  // CHECK: (int(64)) myFormal = 59
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
