use driver, driver_domains, driver_arrays, driver_misc;

config const debug = false;
config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  DomIntType += myIdx;
  DomUintType += myIdx:uintType;
  DomRealType += myIdx:realType;
  var s: string;
  s.write(myIdx);
  DomStringType += s;
}
if debug then writeln(DomIntType);
if debug then writeln(DomUintType);
if debug then writeln(DomRealType);
if debug then writeln(DomStringType);

const AInt2 = AInt;
const AUint2 = AUint;
const AReal2 = AReal;
const AString2 = AString;

testZip(DomIntType, DomIntType);
testZip(DomUintType, DomUintType);
testZip(DomRealType, DomRealType);
testZip(DomStringType, DomStringType);

testZip(DomIntType, AInt);
testZip(DomUintType, AUint);
testZip(DomRealType, AReal);
testZip(DomStringType, AString);

testZip(AInt, DomIntType);
testZip(AUint, DomUintType);
testZip(AReal, DomRealType);
testZip(AString, DomStringType);

testZip(AInt, AInt2);
testZip(AUint, AUint2);
testZip(AReal, AReal2);
testZip(AString, AString2);

proc testZip(D1: domain, D2: domain) {
  type idxType1 = D1._value.idxType;
  type idxType2 = D2._value.idxType;
  var success = true;
  forall (i1, i2) in zip(D1, D2) with (ref success) do
    if i1 != i2 then success = false;
  writeln("Parallel domain-domain zipping ",
          (typeToString(idxType1), typeToString(idxType2)), " : ",
          if success then "SUCCESS" else "FAILED");
}

proc testZip(D1: domain, A2: []) {
  type idxType1 = D1._value.idxType;
  type idxType2 = A2.domain._value.idxType;
  var success = true;
  forall (i1, i2) in zip(D1, A2) with (ref success) do
    if A2(i1) != i2 then success = false;
  writeln("Parallel domain-array zipping ",
          (typeToString(idxType1), typeToString(idxType2)), " : ",
          if success then "SUCCESS" else "FAILED");
}

proc testZip(A1: [], D2: domain) {
  type idxType1 = A1.domain._value.idxType;
  type idxType2 = D2._value.idxType;
  var success = true;
  forall (i1, i2) in zip(A1, D2) with (ref success) do
    if i1 != A1(i2) then success = false;
  writeln("Parallel array-domain zipping ",
          (typeToString(idxType1), typeToString(idxType2)), " : ",
          if success then "SUCCESS" else "FAILED");
}

proc testZip(A1: [], A2: []) {
  type idxType1 = A1.domain._value.idxType;
  type idxType2 = A2.domain._value.idxType;
  var success = true;
  forall (i1, i2) in zip(A1, A2) with (ref success) do
    if i1 != i2 then success = false;
  writeln("Parallel array-array zipping ",
          (typeToString(idxType1), typeToString(idxType2)), " : ",
          if success then "SUCCESS" else "FAILED");
}
