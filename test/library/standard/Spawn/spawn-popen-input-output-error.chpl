use Subprocess;

{
  var comp = spawnshell("$CHPL_HOME/bin/`$CHPL_HOME/util/chplenv/chpl_bin_subdir.py --host`/chpl cat-stdout-stderr.chpl -o stdout-stderr");
  comp.wait();
  assert(comp.exitCode == 0);
}

var sub = spawn(["./stdout-stderr", "-nl", "1"], stdin=pipeStyle.bufferAll, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);

sub.stdin.writeln("Hello");
sub.stdin.writeln("Everybody");
sub.stdin.writeln("Moving");
sub.stdin.writeln("Along");

sub.communicate();

var line:string;
while sub.stdout.readLine(line) {
  write("stdout line: ", line);
}
while sub.stderr.readLine(line) {
  write("stderr line: ", line);
}

assert(sub.running == false);
assert(sub.exitCode == 0);

sub.close();

unlink("stdout-stderr");
