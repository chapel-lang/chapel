 
record R {
  var element:int;
}

var recordTwo = new R(2);

var globalRecord:R;

proc returnGlobalRecord() {
  return globalRecord;
}

writeln("globalRecord is ", globalRecord);
writeln("setting returnGlobalRecord().element = 2");

returnGlobalRecord().element = 2;

// Should this program be legal?
// Should it print out 0 or 2?
writeln(globalRecord.element);
