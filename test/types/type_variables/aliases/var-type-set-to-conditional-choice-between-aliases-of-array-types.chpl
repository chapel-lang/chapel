type t1 = [1..2] int;
type t2 = [1..2] real;

var a: if true then t1 else t2;
writeln(a);
