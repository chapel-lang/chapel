use Futures;

const result: int = 2;

// create a future
var f = buildFuture(int, lambda() { return result; });

// block until value from future is retrieved
var r = f.read();

// this will fail with an error
f.write(4);

// print the results
writeln("F = ", r);
