var y : int = 3;

label l1
var x : int = 12;

writeln(x, y);

y -= 1;

if y > 0 then goto l1;

writeln(x, y);
