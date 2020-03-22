var a : [1..10] int = [10,21,42,17,3,2,55,101,2,3];
var b : [1..10] int = [10,-21,42,-17,3,2,-55,-101,2,3];
var c : [1..10] int;
writeln(minmax reduce a);
writeln(minmax reduce b);
writeln(minmax reduce c);
writeln(minmax scan a);
writeln(minmax scan b);
writeln(minmax scan c);

private use List;
var emptyList = new list(int);
writeln(minmax reduce emptyList);
