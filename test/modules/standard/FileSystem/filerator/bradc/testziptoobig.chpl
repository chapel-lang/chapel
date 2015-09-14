use FileSystem;

forall (i,file) in zip(1..1000, glob("*.chpl")) do
  writeln("file ", i, " is ", file);
