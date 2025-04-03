import CTypes;
extern proc myFunc(): CTypes.c_int;

writeln("Calling myFunc...");
var result = myFunc();
writeln("Result: ", result);
