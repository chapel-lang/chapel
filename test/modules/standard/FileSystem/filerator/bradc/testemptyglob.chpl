use FileSystem;

for file in glob("*.goo") do
  writeln(file);

forall file in glob("*.goo") do
  writeln(file);

forall (file,i) in zip(glob("*.goo"), 1..) do
  writeln("file ", i, " is ", file);

forall (i,file) in zip(1..0, glob("*.goo")) do
  writeln("file ", i, " is ", file);

forall (i,file) in zip(1..10, glob("*.goo")) do
  writeln("file ", i, " is ", file);
