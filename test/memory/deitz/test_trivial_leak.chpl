use Memory;

config var verbose = false;

var u = memoryUsed();

if verbose then
  writeln("Initial Memory: ", u);

var x = 2;
var y = 3;
var z = x + y;
writeln(z);

var uu = memoryUsed();

if verbose then
  writeln("Terminal Memory: ", uu);

if u != uu then
  writeln("Error: memory leak detected");
else
  writeln("Success: no memory leak detected");
