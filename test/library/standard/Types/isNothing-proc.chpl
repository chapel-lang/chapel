use Types;

config const version = 1;

 proc printIsNothing(type t){
   var Var: t;
   if version == 1 then
     writeln(t:string, " ", isNothing(t), " ", isNothing(Var));
   else
     writeln(t:string, " ", isNothingType(t), " ", isNothingValue(Var));
 }
 type nothingType = nothing;
 type intType = int;
 type stringType = string;
 type boolType = bool;
 type bytesType = bytes;
 type isReal = real;
 type isRecord = imag;
 type isComplex = complex;

 printIsNothing(nothingType);
 printIsNothing(intType);
 printIsNothing(stringType);
 printIsNothing(boolType);
 printIsNothing(bytesType); 
 printIsNothing(isReal); 
 printIsNothing(isRecord); 
 printIsNothing(isComplex);
