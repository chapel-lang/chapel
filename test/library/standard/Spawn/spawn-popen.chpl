use Subprocess;

var sub = spawn(["cat", "test.txt"], stdout=pipeStyle.pipe);

var line:string;
while sub.stdout.readLine(line) {
  write("Got line: ", line);
}

sub.wait();
assert(sub.running == false);
assert(sub.exitCode == 0);

sub.close();
