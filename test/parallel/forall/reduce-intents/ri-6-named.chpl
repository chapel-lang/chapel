
config const n = 9999;
var ARR: [1..n] int = 1..n;
var numErrors = 0;

// A (simplified) copy of the predefined SumReduceScanOp.
class UserReduceOp: ReduceScanOp {
  type eltType;
  var value: eltType;

  proc identity         return 0: eltType;
  proc accumulate(elm)  { accumulateOntoState(value, elm); }
  proc accumulateOntoState(ref value, elm)  { value = value + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged UserReduceOp(eltType=eltType);
}

proc check(test:string, expected: int, ri: int, re: int) {
  if ri != expected then
    writeln(test, ":  expected ", expected, "  reduce intent produced ", ri);
  if re != expected then
    writeln(test, ":  expected ", expected, "  reduce expr produced ", re);
  numErrors += (ri != expected) + (re != expected);
}

proc main {
  writeln("n = ", n);

  var maxPos, maxNeg, minPos, minNeg, sumLib, sumUsr: int;
  maxNeg = min(int); minPos = max(int); // others can start at 0

  forall arrElm in ARR with (max reduce maxPos, max reduce maxNeg,
                             min reduce minPos, min reduce minNeg,
                             + reduce sumLib, UserReduceOp reduce sumUsr)
  {
    maxPos = max(maxPos, arrElm);
    maxNeg = max(maxNeg, -arrElm);
    minPos = min(minPos, arrElm);
    minNeg = min(minNeg, -arrElm);
    sumLib = sumLib + arrElm;
    sumUsr = sumUsr + arrElm;
  }

  writeln("forall finished");

  check("maxPos", n, maxPos, max reduce ARR);
  check("maxNeg", -1, maxNeg, max reduce -ARR);
  check("minPos", 1, minPos, min reduce ARR);
  check("minNeg", -n, minNeg, min reduce -ARR);
  check("sumLib", n*(n+1)/2, sumLib, + reduce ARR);
  check("sumUsr", n*(n+1)/2, sumUsr, UserReduceOp reduce ARR);

  if numErrors then
    writeln("NUMERRORS: ", numErrors);
  else
    writeln("success");
}
