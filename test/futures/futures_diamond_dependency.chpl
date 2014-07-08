use Futures;

// dependency between tasks
var f1 = buildFuture(real, lambda() { return 2; });
var f2 = buildFuture(real, lambda() { return f1.read() * 3; });
var f3 = buildFuture(real, lambda() { return f1.read() * 5; });
var f4 = buildFuture(real, lambda() { return f2.read() * f3.read(); });

// block until value from future is retrieved
var r = f4.read();

// print the results
writeln("r = ", r);
