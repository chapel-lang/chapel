use Spawn;

{
  var comp = spawnshell("$CHPL_HOME/bin/`$CHPL_HOME/util/chplenv/chpl_platform.py --host`/chpl cat-stdout-stderr.chpl -o stdout-stderr");
  comp.wait();
  assert(comp.exit_status == 0);
}

var sub = spawn(["./stdout-stderr"], stdin=PIPE, stdout=PIPE, stderr=PIPE);

sub.stdin.writeln("Hello");
sub.stdin.writeln("Everybody");
sub.stdin.writeln("Moving");
sub.stdin.writeln("Along");

sub.communicate();

var line:string;
while sub.stdout.readline(line) {
  write("stdout line: ", line);
}
while sub.stderr.readline(line) {
  write("stderr line: ", line);
}


assert(sub.running == false);
assert(sub.exit_status == 0);

unlink("stdout-stderr");
