def raiseType(type int) type { return real; }
def raiseType(type bool) type { return int; }
var h1:raiseType(int).type;
writeln(typeToString(h1.type));
