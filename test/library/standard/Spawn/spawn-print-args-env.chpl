use Subprocess;

{
  var comp = spawn(["gcc", "print-args-env.c", "-o", "print-args-env"]);
  comp.wait();
  assert(comp.exitCode == 0);
}

{
  var runit = spawn(["./print-args-env", "a", "bc", "def"], ["test=test"]);
  runit.wait();
  assert(runit.exitCode == 0);
}

config const printEnv = false;
if printEnv {
  var runit = spawn(["./print-args-env"]);
  runit.wait();
  assert(runit.exitCode == 0);
}


unlink("print-args-env");

