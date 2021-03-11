use Types;

proc myassert(type t){
var Var: t;
writeln(t:string, " ", isNothing(t), " ", isNothing(Var));
}
type nothingType = nothing;
type intType = int;
type stringType = string;
type boolType = bool;
type bytesType = bytes;
type isReal = real;
type isRecord = imag;
type isComplex = complex;

myassert(nothingType);
myassert(intType);
myassert(stringType);
myassert(boolType);
myassert(bytesType); 
myassert(isReal); 
myassert(isRecord); 
myassert(isComplex);