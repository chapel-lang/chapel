use Subprocess;

{
  var comp = spawn(["chpl", "cat-stdout-stderr.chpl", "-o", "stdout-stderr"]);
  comp.wait();
  assert(comp.exit_status == 0);
}

var sub = spawn(["./stdout-stderr"], stdin=PIPE, stdout=PIPE, stderr=PIPE);

config const n = 10000;
for i in 1..n {
  sub.stdin.writeln(i);
}

sub.communicate();

var i = 1;
var x:int;
while sub.stdout.read(x) {
  //writeln("stdout line: ", x);
  assert(x == i);
  i += 2;
}

i = 2;
while sub.stderr.read(x) {
  //writeln("stderr line: ", x);
  assert(x == i);
  i += 2;
}


assert(sub.running == false);
assert(sub.exit_status == 0);

writeln("OK");

unlink("stdout-stderr");
