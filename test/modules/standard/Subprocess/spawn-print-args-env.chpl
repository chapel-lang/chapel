use Subprocess;

{
  var comp = spawn(["gcc", "print-args-env.c", "-o", "print-args-env"]);
  comp.wait();
  assert(comp.exit_status == 0);
}

{
  var runit = spawn(["./print-args-env", "a", "bc", "def"], ["test=test"]);
  runit.wait();
  assert(runit.exit_status == 0);
}

config const printEnv = false;
if printEnv {
  var runit = spawn(["./print-args-env"]);
  runit.wait();
  assert(runit.exit_status == 0);
}


unlink("print-args-env");

