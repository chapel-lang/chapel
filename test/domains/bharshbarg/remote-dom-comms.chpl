use CommDiagnostics;

const D = {1..20};

proc main() {
  if numLocales <= 1 then halt("This test requires at least two locales.");

  startCommDiagnostics();
  var A : [D] int;
  stopCommDiagnostics();
  
  writeln("Array over local domain:");
  for (loc, data) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", data);
  }
  
  resetCommDiagnostics();
  writeln("--------------------");

  on Locales[numLocales-1] {
    startCommDiagnostics();
    var B : [D] int;
    stopCommDiagnostics();
    writeln("Array over remote domain:");
    for (loc, data) in zip(Locales, getCommDiagnostics()) {
      writeln(loc, ": ", data);
    }
  }
}
