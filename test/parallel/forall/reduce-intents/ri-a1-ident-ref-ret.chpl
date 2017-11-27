// Ensure things are hunky-dory when ReduceOp.identity returns by ref.

config const n = 3;
config const m = 2;

type AT = [1..m] int;

// identity values
const globalIntId: int;
const globalArrId: AT;

var AAA: [1..n] int = 1..n;


const plusOp = new PlusReduceOp(eltType = int);

var xInt: int = 5;
var xArr: AT  = 6;
  
forall aaa in AAA with (
                        PlusReduceOp reduce xInt,
                        PlusReduceOp reduce xArr
                        )
{
  xInt += aaa;
  xArr += aaa;
}

writeln("xInt ", xInt);
writeln("xArr ", xArr);
writeln("globalArrId ", globalArrId);
writeln("globalIntId ", globalIntId);

class PlusReduceOp: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity const ref where eltType == int return globalIntId;
  proc identity const ref where eltType != int return globalArrId;
  proc accumulate(elm)  { value = value + elm; }
  proc accumulateOntoState(ref state, elm) { state = state + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new PlusReduceOp(eltType=eltType);
}
