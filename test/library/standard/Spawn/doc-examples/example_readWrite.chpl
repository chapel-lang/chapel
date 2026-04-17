//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use Subprocess;

var sub = spawn(["cat"], stdin=pipeStyle.bufferAll, stdout=pipeStyle.pipe);

sub.stdin.writeln("Hello");
sub.stdin.writeln("World");

sub.communicate();

var line:string;
while sub.stdout.readLine(line) {
  write("Got line: ", line);
}

// prints out:
// Got line: Hello
// Got line: World
/* STOP_EXAMPLE */
