// slimmed-down version of ri-6-named.chpl
// using 'userReduceInstance' instead of UserReduceOp

config const n = 9999;
var ARR: [1..n] int = 1..n;
var numErrors = 0;

// A (simplified) copy of the predefined SumReduceScanOp.
class UserReduceOp: ReduceScanOp {
  type eltType;
  var value: eltType;

  proc identity         return 0: eltType;
  proc accumulate(elm)  { value = value + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new UserReduceOp(eltType=eltType);
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

  var sumUsr1, sumUsr2: int;
  const userReduceInstance = new UserReduceOp(eltType=int);

  forall arrElm in ARR with (userReduceInstance reduce sumUsr1,
                             new UserReduceOp(eltType=int) reduce sumUsr2)
  {
    sumUsr1 = sumUsr1 + arrElm;
    sumUsr2 += arrElm;
  }

  writeln("forall finished");

  check("sumUsr1", n*(n+1)/2, sumUsr1, UserReduceOp reduce ARR);
  check("sumUsr2", n*(n+1)/2, sumUsr2, UserReduceOp reduce ARR);

  if numErrors then
    writeln("NUMERRORS: ", numErrors);
  else
    writeln("success");
}
