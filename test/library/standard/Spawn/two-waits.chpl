use Spawn;

config const poll=false;
config const pipes=true;
config var buffer=true;

if pipes then
  buffer = true;

var process = if pipes
              then spawn(['ls', 'two-waits.chpl'], stdout=PIPE, stderr=PIPE)
              else spawn(['ls', 'two-waits.chpl']);


if poll {
  while process.running {
    process.poll();
  }
}

process.wait(buffer=buffer);
process.wait(buffer=buffer);

if pipes {
  var s:string;
  process.stdout.readline(s);
  write(s);
}
