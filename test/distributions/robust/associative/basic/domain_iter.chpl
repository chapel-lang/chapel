use driver, driver_domains, driver_misc;
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

testSerial(DomIntType, D, ArithInt, ArithIntRef);
testParallel(DomIntType, D, ArithInt, ArithIntRef);
testSerial(DomUintType, D, ArithUint, ArithUintRef);
testParallel(DomUintType, D, ArithUint, ArithUintRef);
testSerial(DomRealType, D, ArithReal, ArithRealRef);
testParallel(DomRealType, D, ArithReal, ArithRealRef);
if doString {
  testSerial(DomStringType, D, ArithString, ArithStringRef);
  testParallel(DomStringType, D, ArithString, ArithStringRef);
}

proc testSerial(Dom, D, Arr, ArrRef) {
  type idxType = Dom.idxType;
  for ai in Dom {
    if idxType != string then
      Arr(((ai-offset:idxType)/2):int) = ai;
    else {
      var i = (ai.read(int)-offset)/2;
      Arr(i) = ai;
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
  writeln("Serial domain iteration (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}


// could probably use serial true on testSerial
proc testParallel(Dom, D, Arr, ArrRef) {
  type idxType = Dom.idxType;
  forall ai in Dom {
    if idxType != string then
      Arr(((ai-offset:idxType)/2):int) = ai;
    else {
      var i = (ai.read(int)-offset)/2;
      Arr(i) = ai;
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
  writeln("Parallel domain iteration (", idxType:string, ") : ",
          if success then "SUCCESS" else "FAILED");
}
