use Memory;

config var verbose = false;

var u = memoryUsed();

if verbose then
  writeln("Initial Memory: ", u);

def foo() {
  var x: [1..4] int;
  x = 1..4;
  return x(2);
}

var y = foo() + foo() + foo();

var uu = memoryUsed();

writeln(y);

if verbose then
  writeln("Terminal Memory: ", uu);

if u != uu then
  writeln("Error: memory leak detected");
else
  writeln("Success: no memory leak detected");
