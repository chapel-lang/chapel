use Subprocess;

var sub = spawn(["cat"], stdin=pipeStyle.bufferAll, stdout=pipeStyle.pipe);

sub.stdin.writeln("Hello");
sub.stdin.writeln("World");

sub.communicate();

var line:string;
while sub.stdout.readLine(line) {
  write("Got line: ", line);
}

assert(sub.running == false);
assert(sub.exitCode == 0);

sub.close();
