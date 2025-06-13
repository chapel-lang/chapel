//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use IO, Subprocess;

var sub = spawn(["hexdump", "-C"], stdin=pipeStyle.pipe, stdout=pipeStyle.pipe);

// Use 'withSerializer' to create a binary-serializing alias of 'sub.stdin'
var bin = sub.stdin.withSerializer(binarySerializer);

for i in 1..10 do bin.write(i:uint(8));

sub.communicate();

var line : string;
while sub.stdout.readLine(line) do
  write(line);
/* STOP_EXAMPLE */
