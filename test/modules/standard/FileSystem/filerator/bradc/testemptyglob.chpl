use FileSystem;

for file in glob("*.goo") do
  writeln(file);

forall file in glob("*.goo") do
  writeln(file);

forall (file,i) in zip(glob("*.goo"), 1..) do
  writeln("file ", i, " is ", file);

forall (i,file) in zip(1..0, glob("*.goo")) do
  writeln("file ", i, " is ", file);

// When here.maxTaskPar was >= 10, the following loop segfaulted ~50% of the
// time. I have no idea why, but I wanted to quiet the regression. See the
// history for more details.
forall (i,file) in zip(1..here.maxTaskPar+1, glob("*.goo")) do
  writeln("file ", i, " is ", file);
