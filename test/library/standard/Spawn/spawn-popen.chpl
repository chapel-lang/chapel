use Spawn;

var sub = spawn(["cat", "test.txt"], stdout=PIPE);

var line:string;
while sub.stdout.readline(line) {
  write("Got line: ", line);
}

sub.wait();
assert(sub.running == false);
assert(sub.exitCodeUNIQUE == 0);

sub.close();
