 
record R {
  var element:int;
}

var recordTwo = new R(2);

var globalRecord:R;

proc returnGlobalRecord() {
  return globalRecord;
}

writeln("globalRecord is ", globalRecord);
writeln("setting returnGlobalRecord() = recordTwo");

// compile error: illegal lvalue in assignment
returnGlobalRecord() = recordTwo;

// Note though, since records return by value by default,
// if such a statement were not a compilation error,
// it would not modify globalRecord.

