use Subprocess;

var sub = spawn(["cat"], stdin=pipeStyle.pipe);

sub.stdin.writeln("Hello");
sub.stdin.writeln("World");
sub.wait();
assert(sub.running == false);
assert(sub.exitCode == 0);

sub.close();

