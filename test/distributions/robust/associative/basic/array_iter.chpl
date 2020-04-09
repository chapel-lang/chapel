use driver, driver_domains, driver_arrays, driver_misc;
use Sort;

config param doString = false;  // IO not fully defined on string types yet
config const debug = false;
config const offset = 7;

for i in D {
  var myIdx = 2*i+offset;
  ArithIntRef(i) = myIdx;
  DomIntType += myIdx;
  ArithUintRef(i) = myIdx:uintType;
  DomUintType += myIdx:uintType;
  ArithRealRef(i) = myIdx:realType;
  DomRealType += myIdx:realType;
  if doString {
    var s: string;
    s = myIdx:string;
    ArithStringRef(i) = s;
    DomStringType += s;
  }
}
sort(ArithIntRef);
sort(ArithUintRef);
sort(ArithRealRef);
if doString then sort(ArithStringRef);
if debug then writeln(DomIntType);
if debug then writeln(ArithIntRef);
if debug then writeln(DomUintType);
if debug then writeln(ArithUintRef);
if debug then writeln(DomRealType);
if debug then writeln(ArithRealRef);
if doString {
  if debug then writeln(DomStringType);
  if debug then writeln(ArithStringRef);
}

forall ai in DomIntType do
  AInt(ai) = ai;
if debug then writeln(AInt);
forall au in DomUintType do
  AUint(au) = au;
if debug then writeln(AUint);
forall ar in DomRealType do
  AReal(ar) = ar;
if debug then writeln(AReal);
if doString {
  forall aStr in DomStringType do
    AString(aStr) = aStr;
  if debug then writeln(AString);
}

testSerial(AInt, D, ArithInt, ArithIntRef);
testParallel(AInt, D, ArithInt, ArithIntRef);
testSerial(AUint, D, ArithUint, ArithUintRef);
testParallel(AUint, D, ArithUint, ArithUintRef);
testSerial(AReal, D, ArithReal, ArithRealRef);
testParallel(AReal, D, ArithReal, ArithRealRef);
if doString {
  testSerial(AString, D, ArithString, ArithStringRef);
  testParallel(AString, D, ArithString, ArithStringRef);
}

proc testSerial(AAssoc, D, Arr, ArrRef) {
  type idxType = AAssoc.idxType;
  for aa in AAssoc {
    if idxType != string {
      var ai = aa;
      Arr(((ai-offset:idxType)/2):int) = ai;
    } else {
      var ai = aa;
      var i = (aa.read(int)-offset)/2;
      Arr(i) = aa;
    }
  }
  if debug then writeln(Arr);
  var success = true;
  sort(Arr);
  for i in D {
    if Arr(i) != ArrRef(i) {
      success = false;
      break;
    }
  }
  writeln("Serial array iteration (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}

// could probably use serial true on testSerial
proc testParallel(AAssoc, D, Arr, ArrRef) {
  type idxType = AAssoc.idxType;
  forall aa in AAssoc {
    if idxType != string {
      var ai = aa;
      Arr(((ai-offset:idxType)/2):int) = ai;
    } else {
      var ai = aa;
      var i = (aa.read(int)-offset)/2;
      Arr(i) = aa;
    }
  }
  if debug then writeln(Arr);
  var success = true;
  sort(Arr);
  for i in D {
    if Arr(i) != ArrRef(i) {
      success = false;
      break;
    }
  }
  writeln("Serial array iteration (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}

