use Spawn;
const cmd = ["sh", "-c", "echo stderr 1>&2; echo stdout;"];
var sub = spawn(cmd, stdout=PIPE, stderr=STDOUT);
writeln("main process");
sub.wait();
