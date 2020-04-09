// Check that the outer var's initial value is accumulated too.

config const n = 3;

var AAA: [1..n] int = 1..n;

const plusOp = new unmanaged PlusReduceOp(eltType = int);

var xPlus = [400, 500];
var xUser = 600;
var xOp = 700;
  
forall aaa in AAA with (
                        + reduce xPlus,
                        PlusReduceOp reduce xUser,
                        plusOp reduce xOp
                        )
{
  xPlus reduce= aaa;
  xUser reduce= aaa;
  xOp   reduce= aaa;
}

writeln("plus:        ", xPlus);
writeln("user reduce: ", xUser);
writeln("user op:     ", xOp);

delete plusOp;

class PlusReduceOp: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity         return 0: eltType;
  proc accumulate(elm)  { value = value + elm; }
  proc accumulateOntoState(ref state, elm) { state = state + elm; }
  proc initialAccumulate(elm)  { writef("initialAccumulate(%t)\n", elm); accumulate(elm); }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged PlusReduceOp(eltType=eltType);
}
