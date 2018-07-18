// Ensure things are hunky-dory when ReduceOp.identity returns by ref.

config const n = 3;
config const m = 2;

type AT = [1..m] int;

// identity values
const globalIntIdConst: int;
const globalArrIdConst: AT;
var globalIntIdVar: int;
var globalArrIdVar: AT;

// underlying data
var AAA: [1..n] int = 1..n;

// reduce results will be here
var xInt: int = 5;
var xArr: AT  = 6;

writeln("starting first loop");
  
forall aaa in AAA with (
                        PlusReduceOpConst reduce xInt,
                        PlusReduceOpConst reduce xArr
                        )
{
  xInt += aaa;
  xArr += aaa;
}

writeln("after first loop");
writeln("xInt ", xInt);
writeln("xArr ", xArr);
writeln("globalArrIdConst ", globalArrIdConst);
writeln("globalIntIdConst ", globalIntIdConst);
writeln("globalArrIdVar ", globalArrIdVar);
writeln("globalIntIdVar ", globalIntIdVar);

writeln();  
writeln("starting second loop");

forall aaa in AAA with (
                        PlusReduceOpVar reduce xInt,
                        PlusReduceOpVar reduce xArr
                        )
{
  xInt += aaa;
  xArr += aaa;
}

writeln("after second loop");
writeln("xInt ", xInt);
writeln("xArr ", xArr);
writeln("globalArrIdConst ", globalArrIdConst);
writeln("globalIntIdConst ", globalIntIdConst);
writeln("globalArrIdVar ", globalArrIdVar);
writeln("globalIntIdVar ", globalIntIdVar);

// uses *Const identity values
class PlusReduceOpConst: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity const ref where eltType == int return globalIntIdConst;
  proc identity const ref where eltType != int return globalArrIdConst;
  proc accumulate(elm)  { value = value + elm; }
  proc accumulateOntoState(ref state, elm) { state = state + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged PlusReduceOpConst(eltType=eltType);
}

// uses *Var identity values
class PlusReduceOpVar: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity ref where eltType == int return globalIntIdVar;
  proc identity ref where eltType != int return globalArrIdVar;
  proc accumulate(elm)  { value = value + elm; }
  proc accumulateOntoState(ref state, elm) { state = state + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged PlusReduceOpVar(eltType=eltType);
}
