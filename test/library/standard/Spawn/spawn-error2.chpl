use Subprocess;

{
  var sub = spawn(["./some-command-that-does-not-exist"], stdout=pipeStyle.pipe);
  var line:string;
  var count:int;
  while sub.stdout.readLine(line) {
    count += 1;
  }
  sub.wait();
  sub.close();
}
