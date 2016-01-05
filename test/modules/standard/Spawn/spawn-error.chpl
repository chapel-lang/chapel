use Spawn;

{
  var sub = spawn(["./some-command-that-does-not-exist"]);
  sub.wait();
}


