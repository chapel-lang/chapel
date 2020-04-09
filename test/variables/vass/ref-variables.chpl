// Various uses of a ref variable.
//
// Much of 'myInt' and 'myArr' pieces are from Variables.tex.
// I include them here for completeness.


////// alias of a variable //////

var myInt = 51;
ref refInt = myInt;
myInt = 62;
writeln("refInt = ", refInt);
refInt = 73;
writeln("myInt = ", myInt);


////// alias of a ref-returning function //////

proc justVariable() ref  return myInt;
ref refExpr = justVariable();
refExpr = 84;
writeln("myInt = ", myInt);
myInt = 95;
writeln("refExpr = ", refExpr);


////// alias of an array element //////

var myArr: [1..3] int = 51;
ref refArrElm = myArr[2];
myArr[2] = 62;
writeln("refArrElm = ", refArrElm);
refArrElm = 73;
writeln("myArr[2] = ", myArr[2]);


////// alias of a class field //////

class C { var cField = 51; }
var myC = new borrowed C();
ref refClassField = myC.cField;
myC.cField = 62;
writeln("refClassField = ", refClassField);
refClassField = 73;
writeln("cField = ", myC.cField);



////// alias of a record variable and a record field //////

record R { var rField = 51; }
var myR = new R();
ref refToRecord = myR;          // alias the entire record variable
ref refRecField  = myR.rField;   // alias just a field

refToRecord = new R(rField = 62);  // updates 'myR', including its rField
writeln("refRecField = ", refRecField);
refRecField = 73;
writeln("myR = ", myR);
