var dom: domain(1) = {1..10};

var arr = dom.tryCreateArray(int, [i in dom] i*2);
writeln(arr);

arr = dom.tryCreateArray(int, 42);
writeln(arr);
