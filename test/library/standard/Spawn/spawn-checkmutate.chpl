use Spawn;

{
  var comp = spawn(["gcc", "mutate-args.c", "-o", "mutate-args"]);
  comp.wait();
  assert(comp.exitCodeUNIQUE == 0);
}

{
  var runit = spawn(["./mutate-args", "a", "bc", "def"], ["test=test"]);
  runit.wait();
  assert(runit.exitCodeUNIQUE == 0);
}

unlink("mutate-args");

