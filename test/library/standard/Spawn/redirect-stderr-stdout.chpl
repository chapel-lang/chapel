use Spawn;
const cmd = ["sh", "-c", "echo stderr 1>&2; echo stdout;"];
var sub = spawn(cmd, stdout=PIPE, stderr=STDOUT);
sub.wait();
writeln("main process");
