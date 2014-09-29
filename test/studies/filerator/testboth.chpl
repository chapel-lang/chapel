use Filerator;

for dir in walkdirs("subdir") {
  writeln("dir ", dir, " contains:");
  for file in glob(dir+"/*") do
    writeln("  ", file);
}
