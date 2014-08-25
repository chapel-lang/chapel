// Check isClassType, isRecordType, isUnionType.

// Do not delete empty/comment lines - that will confuse .good

// Once these work correctly, move them from here to type-tests.chpl
var snc: sync int;       test("sync", snc);
var sng: single int;     test("single", sng);
compilerError("done"); // no need for a C compile

proc test(param msg, ref v) {
  compilerWarning(msg + " is a class:  " + isClassType(v.type):c_string);
  compilerWarning(msg + " is a record: " + isRecordType(v.type):c_string);
  compilerWarning(msg + " is a union:  " + isUnionType(v.type):c_string);
  compilerWarning(msg + " is a sync:   " + isSyncType(v.type):c_string
                  + " / " + isSync(v));
  compilerWarning(msg + " is a single: " + isSingleType(v.type):c_string
                  + " / " + isSingle(v));
}
