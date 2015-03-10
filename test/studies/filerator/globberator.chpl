use Filerator;

config const pattern = "*";

for f in glob(pattern) do
  writeln(f);
