use Subprocess;

{
  var sub = spawn(["./some-command-that-does-not-exist"]);
  sub.wait();
}


