var a: atomic int;
a.poke(1);
writeln(a.peek());

var ab: atomic bool;
ab.poke(true);
writeln(ab.peek());
