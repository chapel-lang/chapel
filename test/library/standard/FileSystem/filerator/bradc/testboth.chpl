use FileSystem;

for dir in walkDirs("subdir", sort=true) {
  writeln("dir ", dir, " contains:");
  for file in glob(dir+"/*") do
    writeln("  ", file);
}
