// inspired by #19301
// exercises 'perror' in listdir
// the .good assumes the existence of listdir-errors.chpl

use FileSystem;

for file in listdir("no/such/dir/ever") do
  writeln(file);
for file in listdir("listdir-errors.chpl") do
  writeln(file);
