use Spawn;

proc f(s: string) {
  var p = spawnshell(s, stdout=PIPE, stderr=PIPE);
  p.communicate();
  return p;
}

var p = f(['ls', '-l']);
var lines = p.stdout.lines(); // <-- offending expression
