use CommDiagnostics;

const globalDom = {1..10, 1..10};

proc main() {
  const Offsets = {-1..1, -1..1};
  var changingDom = {1..5};

  writeln("===== on-stmt (variable domain) =====");
  on Locales.tail() {
    startCommDiagnostics();
    const dims = changingDom.dims();
    if dims(0).high > 1000 then halt("foo");
    changingDom = {1..10};
    var A : [changingDom] int;
    assert(A.size == 10);
    stopCommDiagnostics();
    A; // keep A alive until here to not count deinit in counts
  }
  writeln(changingDom);
  for (loc, dat) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", dat);
  }
  resetCommDiagnostics();

  writeln("===== on-stmt (const domain) =====");
  on Locales.tail() {
    startCommDiagnostics();
    const dims = Offsets.dims();
    if dims(0).high > 1000 then halt("foo");
    var A : [Offsets] int;
    stopCommDiagnostics();
    A; // keep A alive until here to not count deinit in counts
  }

  for (loc, dat) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", dat);
  }
  resetCommDiagnostics();

  writeln("===== global =====");
  startCommDiagnostics();
  coforall loc in Locales do on loc {
    var A : [globalDom] int;
    A = here.id;
    if numLocales > 1000 then writeln(A);
  }
  stopCommDiagnostics();

  for (loc, dat) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", dat);
  }
}
