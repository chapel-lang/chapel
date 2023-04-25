use ChapelDebugPrint;

var A = [1, 2, 3, 4, 5, 6, 7, 8];

var loc: int;
chpl__testParStart();
writeln(A.find(6));
writeln((A.find(3, loc), loc));
writeln(A.find(9));
writeln((A.find(9, loc), loc));
chpl__testParStop();
