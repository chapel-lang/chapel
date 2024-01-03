var x: sync string;
param p = isCopyableType(x.type);
writeln("isCopyableType(sync string) = " + p:string);
var y: sync string;
param p2 = isCopyableType(y.type);
writeln("isCopyableType(single string) = " + p2:string);

