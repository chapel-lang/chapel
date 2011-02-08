proc raiseType(type int) type { return real; }
//proc raiseType(type bool) type { return int; }
var h1:raiseType(int);
writeln(typeToString(h1.type));
