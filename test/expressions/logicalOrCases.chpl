use CTypes, Regex;
class c { proc foo() do return true; }
record r { proc foo() do return true; }

var myC = new c?();
var myOtherC = new c?();
var myNil: owned c? = nil;

if myC || myOtherC {
  writeln("one of myC or myOtherC is non-nil");
} else {
  writeln("both myC and myOtherC are nil");
}

if myC || myNil {
  writeln("one of myC or myNil is non-nil");
} else {
  writeln("both myC and myNil are nil");
}

if myNil || myNil {
  writeln("one of myNil or myNil is non-nil (?!)");
} else {
  writeln("both myNil and myNil are nil");
}

var myR: r;
var myRPtr = c_ptrTo(myR);
var myNilPtr: c_ptr(r) = nil;

proc produce(x) do {
  writeln("examining ", x);
  return x;
}

proc produce(x: c_ptr(?)) {
  writeln("examining ", if x then "non-nil pointer" else "nil pointer");
  return x;
}

if produce(myNilPtr) || produce(myRPtr) {
  writeln("one of myNilPtr or myRPtr is non-nil");
} else {
  writeln("both myNilPtr and myRPtr are nil");
}

if produce(myRPtr) || produce(myRPtr) {
  writeln("one of myRPtr or myRPtr is non-nil");
} else {
  writeln("both myRPtr and myRPtr are nil");
}

if produce(0) || produce(2) {
  writeln("0 || 2 is true");
} else {
  writeln("0 || 2 is false");
}

if produce(1) || produce(0) {
  writeln("1 || 0 is true");
} else {
  writeln("1 || 0 is false");
}
