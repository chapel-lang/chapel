use Subprocess;
const cmd = ["sh", "-c", "echo stderr 1>&2; echo stdout;"];
var sub = spawn(cmd, stdout=pipeStype.pipe, stderr=pipeStyle.stdout);
var line: string;
while (sub.stdout.readline(line)) {
  write('*'+line);
}
sub.wait();
writeln("main process");
