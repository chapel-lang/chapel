
proc isInt1(          e) param return isIntValue(e);
proc isInt2(const ref e) param return isIntValue(e);
proc isInt3(const     e) param return isIntValue(e);

var myInt: int;
compilerWarning(isInt1(myInt):c_string);
compilerWarning(isInt2(myInt):c_string);
compilerWarning(isInt3(myInt):c_string);

var syInt:  sync int;
compilerWarning(isInt1(syInt):c_string);
compilerWarning(isInt2(syInt):c_string);
compilerWarning(isInt3(syInt):c_string);

compilerError("done");
