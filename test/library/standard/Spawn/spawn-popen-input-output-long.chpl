use Spawn;

var sub = spawn(["cat"], stdin=BUFFERED_PIPE, stdout=PIPE, stderr=FORWARD);

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
  i += 1;
}

assert(sub.running == false);
assert(sub.exit_status == 0);

sub.close();

writeln("OK");

