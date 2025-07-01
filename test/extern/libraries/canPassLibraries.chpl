import CTypes;

config param useRequire = false;
config param requiredLib = "";
if useRequire {
  require requiredLib;
}

extern proc myFunc(): CTypes.c_int;

writeln("Calling myFunc...");
var result = myFunc();
writeln("Result: ", result);
