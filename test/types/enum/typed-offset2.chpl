type t = int;
enum Numbers {one=1, two=2: t, three, four, five, six = two:int*3, seven};

var x = 2;
var y = x:Numbers;
writeln(y);
writeln(3:Numbers);
writeln(1:Numbers);
writeln(4:Numbers);
writeln(5:Numbers);
writeln(6:Numbers);
writeln(7:Numbers);
