use BlockDist; // any domain map, really

const D = [1..2] dmapped Block({1..2});

var A1: [D] int;
var A2 = [D] 0;

compilerWarning(typeToString(A1.domain._value.type));
compilerWarning(typeToString(A2.domain._value.type));
compilerAssert(A1.type == A2.type);
