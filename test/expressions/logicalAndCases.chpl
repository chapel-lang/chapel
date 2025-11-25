use CTypes, Regex;
class c { proc foo() do return true; }
record r { proc foo() do return true; }

var myC = new c?();
var myNil: owned c? = nil;

if myC && myC!.foo() {
  writeln("myC is non-nil and foo() returned true");
} else {
  writeln("myC is nil or foo() returned false");
}

if myNil && myNil!.foo() {
  writeln("myNil is non-nil and foo() returned true");
} else {
  writeln("myNil is nil or foo() returned false");
}

var myR: r;
var myRPtr = c_ptrTo(myR);
var myNilPtr: c_ptr(r) = nil;

if myRPtr && myRPtr.deref().foo() {
  writeln("myRPtr is non-nil and foo() returned true");
} else {
  writeln("myRPtr is nil or foo() returned false");
}

if myNilPtr && myNilPtr.deref().foo() {
  writeln("myNilPtr is non-nil and foo() returned true");
} else {
  writeln("myNilPtr is nil or foo() returned false");
}

proc produce(x) do {
  writeln("examining ", x);
  return x;
}

if produce(1) && produce(2) {
  writeln("1 && 2 is true");
} else {
  writeln("1 && 2 is false");
}

if produce(1) && produce(0) {
  writeln("1 && 0 is true");
} else {
  writeln("1 && 0 is false");
}

if produce(0) && produce(2) {
  writeln("0 && 2 is true");
} else {
  writeln("0 && 2 is false");
}

var myRegex = new regex("some");
var match = myRegex.match("some string");
if match && match.numBytes {
  writeln("found a match and numBytes is ", match.numBytes);
} else {
  writeln("no match or numBytes is zero");
}
