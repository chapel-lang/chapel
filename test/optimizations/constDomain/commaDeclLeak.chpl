// For the cases below, callDestructors removes initCopys while avoiding the
// destruction for those domains. However, constant domain optimization's AST
// modifications interfered with the AST pattern that resulted in some initCopys
// to be not dropped. This caused even numbered domains below to leak:

const myRange = 1..10;
var dRangeVar1 = {myRange},
    dRangeVar2 = {myRange},
    dRangeVar3 = {myRange},
    dRangeVar4 = {myRange},
    dRangeVar5 = {myRange},
    dRangeVar6 = {myRange};

writeln(dRangeVar1);
writeln(dRangeVar2);
writeln(dRangeVar3);
writeln(dRangeVar4);
writeln(dRangeVar5);
writeln(dRangeVar6);

