type t = int;
enum Numbers {one, two=2: t, three};

var x = 2;
var y = x:Numbers;
writeln(y);
writeln(3:Numbers);
writeln(1:Numbers);
