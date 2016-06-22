var A: [1..3]int;

type tuptype1   = for param i in 1..3 do if i == 1 then int else real;
type tuptype2   = for param i in 1..3 do if i != 1 then int;

param tupparam1 = for param i in 1..3 do i;
param tupparam2 = for param i in 1..3 do if i != 1 then i;

var tupval1     = for param i in 1..3 do A(i);
var tupval2     = for param i in 1..3 do if i != 1 then A(i);

writeln(tuptype1:string);
writeln(tuptype2:string);
writeln(tupparam1);
writeln(tupparam2);
writeln(tupval1);
writeln(tupval2);
