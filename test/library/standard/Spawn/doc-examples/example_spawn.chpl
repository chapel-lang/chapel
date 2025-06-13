//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use Subprocess;

var sub = spawn(["ls", "test_directory"]);
sub.wait();
/* STOP_EXAMPLE */
