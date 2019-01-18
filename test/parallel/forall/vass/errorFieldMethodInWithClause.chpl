// see also parallel/taskPar/vass/errorFieldMethodInWithClause.chpl
const ITER = 1..3;

proc ourProc { return 7; }
iter ourIter { yield 8; }

proc something {}

class myClass {
  var myField = 5;
  proc myProcc return 6;
}

proc myClass.test {
  forall ITER with (in myField) do
    something();
  forall ITER with (in myProcc) do
    something;
  forall ITER with (ref ourProc, ref ourIter) do
    something;
}

class myRecord {
  var myFild = 5;
  proc myProc return 6;
}

proc myRecord.test {
  forall ITER with (in myFild) do
    something();
  forall ITER with (in myProc) do
    something;
  forall ITER with (ref ourProc, ref ourIter) do
    something;
}

proc main {
  const mc = new borrowed myClass();
  mc.test;
  const mr = new borrowed myRecord();
  mr.test;
}
