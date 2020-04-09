
iter myIterator(n:int) {
  writeln("myIterator serial");
  yield n;
}

iter myIterator(n:int, param tag) where tag == iterKind.leader {
  writeln("myIterator leader");
  yield 777;
}

iter myIterator(n:int, param tag, followThis) {
  writeln("myIterator follower ", followThis);
  yield n;
}

proc myPlusOp(x:int,y:int) return x+y+7;

var A,B: [-1..1] int;

proc myTestProc(IR) {
  // todo: same with a zippered forall, with IR either leader or follower
  forall IDX in IR {
    writeln("myTestProc ", IDX);
  }

  // The following works, however.
  var ARR = IR;
  compilerWarning(ARR.type:string);
  writeln("ARR = ", ARR, "  ", ARR.domain);
}

proc main {
  myTestProc(A+B);
  myTestProc(myPlusOp(A,B));
  myTestProc(myIterator(5));
  myTestProc(for i in A do i * 3);
  myTestProc(forall i in A do i * 3);
}
