config param arg1 = 33;
config param arg2 = -55;

compilerWarning("abs(arg1) = ", abs(arg1):string, 0);
compilerWarning("abs(arg2) = ", abs(arg2):string, 0);
compilerError("no need to codegen", 0);
