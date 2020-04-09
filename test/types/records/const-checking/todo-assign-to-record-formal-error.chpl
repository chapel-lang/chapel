// For records passed by blank intent,
// should not allow assigning to their fields.

record MyRecord {
  var myField: int;
}

proc myProc(y: MyRecord) {
  y.myField = 7;  // should be an error
  writeln(y);
}

var x = new MyRecord(5);
myProc(x);

compilerError("done"); // no need to execute
