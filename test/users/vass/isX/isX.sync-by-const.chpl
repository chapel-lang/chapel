
proc isInt1(          e) param do return isIntValue(e);
proc isInt2(const ref e) param do return isIntValue(e);
proc isInt3(const     e) param do return isIntValue(e);

var myInt: int;
compilerWarning(isInt1(myInt):string);
compilerWarning(isInt2(myInt):string);
compilerWarning(isInt3(myInt):string);

var syInt:  sync int;
compilerWarning(isInt1(syInt):string);
compilerWarning(isInt2(syInt):string);
compilerWarning(isInt3(syInt):string);

compilerError("done");
