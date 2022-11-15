var x: sync string;
param p = isCopyableType(x.type);
compilerError("isCopyableType(sync string) = " + p:string);
