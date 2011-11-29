var A3 : [1..10, 1..10] int;
writeln (A3(1, ..));

var A4 : [1 : int(64) .. 10: int(64), 1 : int(64) .. 10: int(64)] int;
writeln (A4(1,1));

//this line generates the error message
writeln (A4(1, ..));
