use Spawn;

{
  var sub = spawn(["./some-command-that-does-not-exist"], stdout=PIPE);
  var line:string;
  var count:int;
  while sub.stdout.readline(line) {
    count += 1;
  }
  sub.wait();
  sub.close();
}


