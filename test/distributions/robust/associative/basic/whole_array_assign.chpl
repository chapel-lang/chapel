use driver, driver_domains, driver_arrays, driver_misc;
use Sort;

config const debug = false;
config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  DomIntType += myIdx;
  DomUintType += myIdx:uintType;
  DomRealType += myIdx:realType;
  var s: string;
  s = myIdx:string;
  DomStringType += s;
}
if debug then writeln(DomIntType);
if debug then writeln(DomUintType);
if debug then writeln(DomRealType);
if debug then writeln(DomStringType);

forall ai in DomIntType do
  AInt(ai) = ai;
if debug then writeln(AInt);
forall au in DomUintType do
  AUint(au) = au;
if debug then writeln(AUint);
forall ar in DomRealType do
  AReal(ar) = ar;
if debug then writeln(AReal);
forall aStr in DomStringType do
  AString(aStr) = aStr;
if debug then writeln(AString);

testWhole(AInt);
testWhole(AUint);
testWhole(AReal);
testWhole(AString);

proc testWhole(AAssoc) {
  type idxType = AAssoc.domain.idxType;
  var AA: AAssoc.type;
  if debug then writeln(AA);
  AA = AAssoc;
  if debug then writeln(AA);
  var success = true;
  for ai in AAssoc.domain {
    if AA(ai) != AAssoc(ai) {
      success = false;
      break;
    }
  }
  writeln("Whole array assignment (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}

