use CommDiagnostics;

// locale array initalization requires the defaultLocale to be read, we have to
// make sure that this doesn't incur communication
for l in Locales do on l {
  startCommDiagnosticsHere();
  var myLocales: [1..1] locale;
  stopCommDiagnosticsHere();
  writeln(here, " myLocales values are default : ",
          myLocales[1] == Locales[0]);
  writeln(here, " Comm Diagnostics : ", getCommDiagnosticsHere());
  resetCommDiagnosticsHere();
}

writeln();

// we don't know how long we'll be supporting nilLocale or even whether that's
// user-facing or not (probably not). But reading the nilLocale (or any locale
// value that the user might wanna read) shouldn't cause any communication
for l in Locales do on l {
  startCommDiagnosticsHere();
  const isNilLocaleNil = nilLocale._instance == nil;
  stopCommDiagnosticsHere();
  writeln(here, " isNilLocaleNil : ", isNilLocaleNil);
  writeln(here, " Comm Diagnostics : ", getCommDiagnosticsHere());
  resetCommDiagnosticsHere();
}
