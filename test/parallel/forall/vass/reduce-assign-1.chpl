
config const n = 8;

var A: [1..n] int = 1..n;
var B1: [1..n] bool = true; B1[2] = false;
var B2: [1..n] bool = false; B2[2] = true;

proc showBits(name: string, arg: int) {
  writef("%s %08bu\n", name, arg);
}

proc main {

  var xSum = 0,
      xProd = 1,
      xMax = min(int),
      xMin = max(int),
      bAnd = -1,
      bOr  = 0,
      bXor = 0,
      xUser = 0;

  forall a in A with (+ reduce xSum, * reduce xProd,
                      max reduce xMax, min reduce xMin,
                      & reduce bAnd, | reduce bOr, ^ reduce bXor,
                      PlusReduceOp reduce xUser)
  {
    xSum  reduce= a;
    xProd reduce= a;
    xMax  reduce= a;
    xMin  reduce= a;
    bAnd  reduce= a | 9;
    bOr   reduce= a;
    bXor  reduce= 1 << 2*a;
    xUser reduce= a;
  }

  writeln("+ reduce:   ", xSum);
  writeln("* reduce:   ", xProd);
  writeln("max reduce: ", xMax);
  writeln("min reduce: ", xMin);
  showBits("& reduce:  ", bAnd);
  showBits("| reduce:  ", bOr);
  showBits("^ reduce:  ", bXor);
  writeln("user reduce: ", xUser);

  var lAnd = true, lOr = false;

  forall (b1,b2) in zip(B1,B2) with (&& reduce lAnd, || reduce lOr) {
    lAnd reduce= b1;
    lOr reduce= b2;
  }

  writeln("&& reduce:   ", lAnd);
  writeln("|| reduce:   ", lOr);

}

class PlusReduceOp: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity         return 0: eltType;
  proc accumulate(elm)  { value = value + elm; }
  proc accumulateOntoState(ref state, elm) { state = state + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged PlusReduceOp(eltType=eltType);
}
