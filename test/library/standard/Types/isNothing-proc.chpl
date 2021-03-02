use Types;

proc isNothing(type t)  param return isNothingType(t);
proc isNothing(e)   param return isNothingType(e.type);


proc myassert(param isNothing){
writeln(isNothing);
}
type nothingType = nothing;
type intType = int;
type stringType = string;
type boolType = bool;
type bytesType = bytes;

var nothingVar = none;
var intVar = 20;
var stringVar = "none";
var boolVar = true;
var bytesVar = 3.14;

myassert(isNothing(nothingType));
myassert(isNothing(intType));
myassert(isNothing(stringType));
myassert(isNothing(boolType));
myassert(isNothing(bytesType));
 
myassert(isNothing(nothingVar));
myassert(isNothing(intVar));
myassert(isNothing(stringVar));
myassert(isNothing(boolType));
myassert(isNothing(bytesVar));