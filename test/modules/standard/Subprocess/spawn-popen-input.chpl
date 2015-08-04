use Subprocess;

var sub = spawn(["cat"], stdin=PIPE);

sub.stdin.writeln("Hello");
sub.stdin.writeln("World");
sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


