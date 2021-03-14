use FileSystem;

config const pattern = "*";

forall f in glob(pattern) do
  writeln(f);
