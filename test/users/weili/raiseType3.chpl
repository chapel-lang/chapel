proc raiseType(type t:int) type { return real; }
proc raiseType(type t:bool) type { return int; }
var h1:raiseType(int);
writeln(h1.type:string);
