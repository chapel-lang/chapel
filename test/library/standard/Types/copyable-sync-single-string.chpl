var x: sync string;
param p = isCopyableType(x.type);
compilerWarning("isCopyableType(sync string) = " + p:string);
var y: sync string;
param p2 = isCopyableType(y.type);
compilerWarning("isCopyableType(single string) = " + p2:string);

