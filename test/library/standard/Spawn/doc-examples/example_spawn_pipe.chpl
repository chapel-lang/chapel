//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use Subprocess;

var sub = spawn(["ls", "test_directory"], stdout=pipeStyle.pipe);

var line:string;
while sub.stdout.readLine(line) {
  write("ls returned: ", line);
}

sub.wait();
/* STOP_EXAMPLE */
