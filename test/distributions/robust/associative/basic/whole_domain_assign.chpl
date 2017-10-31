//
// NOTE: Parallel whole-domain assignment is not yet implemented
//
use driver, driver_domains, driver_misc;
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

testWhole(DomIntType);
testWhole(DomUintType);
testWhole(DomRealType);
testWhole(DomStringType);

proc testWhole(DA) {
  type idxType = DA.idxType;
  var DA2: DA.type;
  if debug then writeln(DA2);
  DA2 = DA;
  if debug then writeln(DA2);
  var success = true;
  for (ai, ai2) in zip(DA.sorted(), DA2.sorted()) {
    if ai != ai2 {
      success = false;
      break;
    }
  }
  writeln("Whole domain assignment (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}

