var y : integer = 3;

label l1
var x : integer = 12;

writeln(x, y);

y -= 1;

if y > 0 then goto l1;

writeln(x, y);
