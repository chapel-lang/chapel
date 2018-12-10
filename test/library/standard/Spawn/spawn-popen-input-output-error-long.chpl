use Spawn;

{
  var comp = spawnshell("$CHPL_HOME/bin/`$CHPL_HOME/util/chplenv/chpl_bin_subdir.py --host`/chpl cat-stdout-stderr.chpl -o stdout-stderr");
  comp.wait();
  assert(comp.exit_status == 0);
}

var sub = spawn(["./stdout-stderr", "-nl", "1"], stdin=BUFFERED_PIPE, stdout=PIPE, stderr=PIPE);

config const n = 10000;
for i in 1..n {
  sub.stdin.writeln(i);
}

sub.communicate();

var i = 1;
var x:int;
while sub.stdout.read(x) {
  if x != i then
    writeln("got stdout ", x, " expected ", i);
  assert(x == i);
  i += 2;
}

i = 2;
while sub.stderr.read(x) {
  if x != i then
    writeln("got stderr ", x, " expected ", i);
  assert(x == i);
  i += 2;
}


assert(sub.running == false);
assert(sub.exit_status == 0);

sub.close();

writeln("OK");

unlink("stdout-stderr");
