use Subprocess;

{
  var comp = spawn(["gcc", "mutate-args.c", "-o", "mutate-args"]);
  comp.wait();
  assert(comp.exitCode == 0);
}

{
  var runit = spawn(["./mutate-args", "a", "bc", "def"], ["test=test"]);
  runit.wait();
  assert(runit.exitCode == 0);
}

unlink("mutate-args");

