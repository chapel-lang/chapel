use Filerator;

config const pattern = "*";
config const useglob = true;

if useglob then
  for f in glob(pattern) do
    writeln(f);
else
  for f in wordexp(pattern) do
    writeln(f);
