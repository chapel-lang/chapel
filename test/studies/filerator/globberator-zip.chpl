use Filerator;

config const pattern = "*.chpl";

forall (f,i) in zip(glob(pattern), 1..) do
  writeln("File #", i, " is: ", f);

//
// Hardcoding number of matches in this directory
//
forall (i,f) in zip(1..9, glob(pattern)) do
  writeln("File #", i, " is: ", f);
