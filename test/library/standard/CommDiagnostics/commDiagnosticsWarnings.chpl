use CommDiagnostics;
use ChplConfig only compiledForSingleLocale;
use IO only stderr;

stderr.writeln("global start/stop");

startVerboseComm();
startVerboseComm(); // triggers warning
stopVerboseComm();
stopVerboseComm(); // trigers warning

startCommDiagnostics();
startCommDiagnostics(); // triggers warning
stopCommDiagnostics();
stopCommDiagnostics(); // trigers warning

if compiledForSingleLocale() || numLocales == 1 {
  stderr.writeln("here start/stop");
  startVerboseCommHere();
  startVerboseCommHere(); // triggers warning
  stopVerboseCommHere();
  stopVerboseCommHere(); // trigers warning

  startCommDiagnosticsHere();
  startCommDiagnosticsHere(); // triggers warning
  stopCommDiagnosticsHere();
  stopCommDiagnosticsHere(); // trigers warning

} else if (numLocales == 4) {
  // test is built for 4 locales

  //
  // test starting and stopping in order on all locales
  //
  stderr.writeln("all here start/stop");
  for l in Locales do on l do startVerboseCommHere();
  for l in Locales do on l do startVerboseCommHere(); // triggers warning
  for l in Locales do on l do stopVerboseCommHere();
  for l in Locales do on l do stopVerboseCommHere(); // trigers warning

  for l in Locales do on l do startCommDiagnosticsHere();
  for l in Locales do on l do startCommDiagnosticsHere(); // triggers warning
  for l in Locales do on l do stopCommDiagnosticsHere();
  for l in Locales do on l do stopCommDiagnosticsHere(); // trigers warning

  //
  // starting on locale 0, then stopping on locale 1 should warn
  //
  stderr.writeln("different locale start/stop");
  on Locales[0] do startVerboseCommHere();
  on Locales[1] do stopVerboseCommHere(); // triggers warning

  on Locales[0] do startCommDiagnosticsHere();
  on Locales[1] do stopCommDiagnosticsHere(); // triggers warning
}
