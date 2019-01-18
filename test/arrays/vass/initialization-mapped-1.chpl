use BlockDist; // any domain map, really

const D = {1..2} dmapped Block({1..2});

var A1: [D] int;
var A2 = [D] 0;

compilerWarning(A1.domain.type:string);
compilerWarning(A2.domain.type:string);
compilerAssert(A1.type == A2.type);
compilerError("done"); // enough checking
