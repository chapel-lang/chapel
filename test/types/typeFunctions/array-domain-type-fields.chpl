var D = {1..10};
var A:[D] int;

// this equivalent of the below works
//writeln(D.idxType:string);
//writeln(D.rank:string);
writeln(D.type.idxType:string);
writeln(D.type.rank:string);

// this equivalent of the below works
//writeln(A.eltType:string);
//writeln(A.idxType:string);
//writeln(A.rank:string);
writeln(A.type.eltType:string);
writeln(A.type.idxType:string);
writeln(A.type.rank:string);
