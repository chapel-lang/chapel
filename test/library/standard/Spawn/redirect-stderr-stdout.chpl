use Spawn;
const cmd = ["sh", "-c", "echo stderr 1>&2; echo stdout;"];
var sub = spawn(cmd, stdout=PIPE, stderr=STDOUT);
var line: string;
while (sub.stdout.readline(line)) {
  write('*'+line);
}
sub.wait();
writeln("main process");
