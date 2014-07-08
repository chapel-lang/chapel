use Futures;

// future tasks
var f1 = buildFuture(int, lambda() { return 2; });
var f2 = buildFuture(int, lambda() { return 3; });
var f3 = buildFuture(real, lambda() { return 5; });

// block until value from future is retrieved
var r1 = f1 + f2;
var r2 = f2 + f3;

// print the results
writeln("r1 = ", r1);
writeln("r2 = ", r2);
