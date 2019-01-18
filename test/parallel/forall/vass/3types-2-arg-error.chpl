// Exercise different types for input, state, output.

config const n = 99;

///////////

record Rinput {
  type eltType;
  var inField: eltType;
}

record Rstate {
  type eltType;
  var stField: chpl__sumType(eltType);
}

record Routput {
  type eltType;
  var outField: chpl__sumType(eltType);
}

class MyOp: ReduceScanOp {
  type eltType;

  var  value: Rstate(eltType);

  proc identity           return new Rstate(eltType);
  proc accumulate(state)  { value.stField += state.stField; }
  proc accumulateOntoState(ref state, elm)   { state.stField += elm; }
  proc combine(other)     { value.stField += other.value.stField; }
  proc generate()         return new Routput(eltType, value.stField);
  proc clone()            return new unmanaged MyOp(eltType=eltType);
}

///////////

type MyInput = int;

var A = [i in 1..n] new Rinput(MyInput, i);

///////////

proc main {
  var xxx: Routput(MyInput);

  forall a in A with  (MyOp(MyInput,MyInput) reduce xxx)
  {
    compilerWarning(xxx.type:string,0);
    xxx reduce= a.inField;
  }

  writeln("result = ", xxx.outField);
}
