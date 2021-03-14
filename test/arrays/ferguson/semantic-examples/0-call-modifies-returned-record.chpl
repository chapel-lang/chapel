 
record R {
  var element:int;
}

var recordTwo = new R(2);

var globalRecord:R;

proc returnGlobalRecord() {
  return globalRecord;
}

proc setToTwo(ref r:R) {
  r = new R(2);
}


writeln("globalRecord is ", globalRecord);
writeln("setting returnGlobalRecord() = recordTwo");

// compile error: illegal lvalue in assignment
setToTwo(returnGlobalRecord());

// Note though, since records return by value by default,
// if such a statement were not a compilation error,
// it would not modify globalRecord.

