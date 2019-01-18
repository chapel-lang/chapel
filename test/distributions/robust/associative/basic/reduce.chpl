use driver, driver_domains, driver_arrays, driver_misc;
use Sort;

config const debug = false;
config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  DomIntType += myIdx;
  DomUintType += myIdx:uintType;
  DomRealType += myIdx:realType;
}
if debug then writeln(DomIntType);
if debug then writeln(DomUintType);
if debug then writeln(DomRealType);

forall ai in DomIntType do
  AInt(ai) = ai;
if debug then writeln(AInt);
forall au in DomUintType do
  AUint(au) = au;
if debug then writeln(AUint);
forall ar in DomRealType do
  AReal(ar) = ar;
if debug then writeln(AReal);

testReduceArr(AInt);
testReduceDom(DomIntType, AInt);
testReduceArr(AUint);
testReduceDom(DomUintType, AUint);
testReduceArr(AReal);
testReduceDom(DomRealType, AReal);

proc testReduceArr(AA) {
  type idxType = AA.domain.idxType;
  var minAA = min reduce AA;
  var checkMin = max(idxType);
  for ai in AA.domain {
    if AA(ai) < checkMin then
      checkMin = AA(ai);
  }
  writeln("Parallel array reduce (", idxType:string, ") : ",
          if checkMin==minAA then "SUCCESS" else "FAILED");
}

proc testReduceDom(Dom, AA) {
  type idxType = Dom.idxType;
  var minAA = min reduce [ai in Dom] AA(ai);
  var checkMin = max(idxType);
  for ai in AA.domain {
    if AA(ai) < checkMin then
      checkMin = AA(ai);
  }
  writeln("Parallel domain reduce (", idxType:string, ") : ",
          if checkMin==minAA then "SUCCESS" else "FAILED");
}

