use Spawn;

{
  var comp = spawn(["gcc", "return-10.c", "-o", "return-10"]);
  comp.wait();
  assert(comp.exitCodeUNIQUE == 0);
}

{
  var runit = spawn(["./return-10"]);
  runit.wait();
  assert(runit.exitCodeUNIQUE == 10);
}

unlink("return-10");

