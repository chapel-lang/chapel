use LinkedLists;

var s = makeList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

writeln(s);
writeln(+ reduce s);

s.destroy();
