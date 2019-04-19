use FileSystem;

for f in listdir('./DOES-NOT-EXIST') {
  writeln(f);
}
