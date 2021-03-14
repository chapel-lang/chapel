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

testPromote(AInt);
testPromote(AUint);
testPromote(AReal);

proc foo(x: real, y: real) {
  return sqrt(x**2 + y**2);
}

proc testPromote(AAssoc) {
  type idxType = AAssoc.domain.idxType;
  var AA: AAssoc.type;
  if debug then writeln(AA);
  AA = foo(AAssoc, AAssoc):idxType;
  if debug then writeln(AA);
  var success = true;
  for ai in AA.domain {
    if AA(ai) != foo(AAssoc(ai), AAssoc(ai)):idxType {
      success = false;
      break;
    }
  }
  writeln("Zippered promotion (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}

