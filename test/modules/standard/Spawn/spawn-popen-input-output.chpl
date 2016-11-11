use Spawn;

var sub = spawn(["cat"], stdin=BUFFERED_PIPE, stdout=PIPE);

sub.stdin.writeln("Hello");
sub.stdin.writeln("World");

sub.communicate();

var line:string;
while sub.stdout.readline(line) {
  write("Got line: ", line);
}

assert(sub.running == false);
assert(sub.exit_status == 0);

sub.close();

