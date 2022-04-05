use Subprocess;

proc f(s: string) {
  var p = spawnshell(s, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
  p.communicate();
  return p;
}

var p = f(['ls', '-l']);
var lines = p.stdout.lines(); // <-- offending expression
