use Subprocess;

//var sub = spawn(["cat"]);
var sub = spawn(["cat"], stdin=PIPE);
//var sub = spawn(["./cat-stderr"], stdin=PIPE);
//var sub = spawn(["./junk"], stdin=PIPE);

sub.stdin.writeln("Hello");
sub.stdin.writeln("World");
sub.stdin.flush();
sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


