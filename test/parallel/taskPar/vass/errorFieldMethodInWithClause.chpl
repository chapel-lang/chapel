// see also field-in-task-ref-clause.chpl
// see also parallel/forall/vass/errorFieldMethodInWithClause.chpl

const ITER = 1..3;

class myClass {
  var myField = 5;
  proc myProcc return 6;
}

proc ourProc { return 7; }
iter ourIter { yield 8; }

proc something {}

proc myClass.test {


  // forall testing moved to parallel/forall/vass


  cobegin with (in myField) {
    something;
    something;
  }
  cobegin with (in myProcc) {
    something;
    something;
  }
  cobegin with (ref ourProc, ref ourIter) {
    something;
    something;
  }

  coforall ITER with (in myField) do
    something;
  coforall ITER with (in myProcc) do
    something;
  coforall ITER with (ref ourProc, ref ourIter) do
    something;

 sync {
  begin with (in myField)
    { something;  }
  begin with (in myProcc)
    { something;  }
  begin with (ref ourProc, ref ourIter)
    { something;  }
 }
}

proc main {
  const mc = new owned myClass();
  mc.test;
}
