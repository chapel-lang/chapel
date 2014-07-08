use Futures;

const result: int = 2;

// create a future
var f = buildFuture(int, lambda() { return result; });

// block until value from future is retrieved
var r = f.read();

// print the results
writeln("F = ", r);
