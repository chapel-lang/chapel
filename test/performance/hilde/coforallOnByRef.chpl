// coforallOnByRef.chpl
//
// This program demonstrates that reference-counted variables shared across a
// coforall need not have their reference counts adjusted.
//
// [copied from streamEP.chpl]
//

use CommDiagnostics;

proc main() {
  var validAnswers: [LocaleSpace] bool;
resetCommDiagnostics();
startCommDiagnostics();
  coforall loc in Locales do on loc {
    validAnswers[here.id] = true;
  }
stopCommDiagnostics();
writeln(getCommDiagnostics());
}
