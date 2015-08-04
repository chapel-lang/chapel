use Subprocess;

var sub = spawn(["cat", "test.txt"], stdout=PIPE);

var line:string;
while sub.stdout.readline(line) {
  writeln("Got line: ", line);
}

sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);


