use BlockDist;
use CommDiagnostics;

proc main() {
  var Dom = {1..100};
  var Space = Dom dmapped Block(Dom);
  var A, B, C : [Space] real = 0;

  startCommDiagnostics();
  A = B + C;
  stopCommDiagnostics();

  for (loc, data) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", data);
  }
}
