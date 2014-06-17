// Check isClassType, isRecordType, isUnionType.


// Once these work correctly, move them from here to type-tests.chpl
var atm: atomic int;     test("atomic", atm);
var snc: sync int;       test("sync", snc);
var sng: single int;     test("single", sng);
compilerError("done"); // no need for a C compile

proc test(param msg, ref v) {
  compilerWarning(msg + " is a class:  " + isClassType(v.type):string);
  compilerWarning(msg + " is a record: " + isRecordType(v.type):string);
  compilerWarning(msg + " is a union:  " + isUnionType(v.type):string);
}
