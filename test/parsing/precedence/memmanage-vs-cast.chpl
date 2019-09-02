
class C { }

// how to interpret owned C: borrowed class
// if it is (owned C):(borrowed class) then the result is borrowed
// if it is owned (C: borrowed class) then the result is owned / duplicate mgmt

compilerWarning((owned C: borrowed class):string);   // expect borrowed C
compilerWarning(((owned C): borrowed class):string); // expect borrowed C
compilerWarning((owned (C: borrowed class)):string); // expect owned C

compilerError("done");
