use FileSystem;
use Subprocess;

{
  var comp = spawn(["gcc", "return-10.c", "-o", "return-10"]);
  comp.wait();
  assert(comp.exitCode == 0);
}

{
  var runit = spawn(["./return-10", ]);
  runit.wait();
  assert(runit.exitCode == 10);
}

FileSystem.remove("return-10");

