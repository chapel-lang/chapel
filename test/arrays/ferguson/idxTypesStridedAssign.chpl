
var b:  [0:uint(64)..4:uint(64) by 2] real = (1.1, 3.3, 5.5);
var bb: [0:uint(64)..4:uint(64) by 2] real;

bb = b;
writeln("bb is: ", bb);

var c:  [2:uint(64)..6:uint(64) by -2] real = (1.1, 3.3, 5.5);
var cc: [2:uint(64)..6:uint(64) by -2] real;
cc = c;
writeln("cc is: ", cc);

var d = {0:uint(64)..4:uint(64) by 2};
var dd = {2:uint(64)..4:uint(64) by 2};

dd = d;
writeln("dd is: ", dd);

var e = {2:uint(64)..6:uint(64) by -2};
var ee = {2:uint(64)..4:uint(64) by -2};

ee = e;
writeln("ee is: ", ee);
