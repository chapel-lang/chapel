use CommDiagnostics;
use Regex;

proc preventRvf(x) {
  /* this is always true but makes sure rvf won't fire */
  assert(x.locale.id >= 0);
}

proc serialize(type t, numChars) {
  writef("serialize(%s, %n)\n", t:string, numChars);
  resetCommDiagnostics();

  var r = compile(("x":t)*numChars);
  startCommDiagnostics();
  on Locales[1] {
    var r2 = r;
    assert(r2.match(("x":t)*numChars).matched);
  }
  stopCommDiagnostics();
  writeln(getCommDiagnostics());
}

proc remoteMatch(type t, numChars) {
  writef("match(%s, %n)\n", t:string, numChars);
  resetCommDiagnostics();

  var r = compile(("x":t)*numChars);
  on Locales[1] {
    preventRvf(r);
    startCommDiagnostics();
    assert(r.match(("x":t)*numChars).matched);
    stopCommDiagnostics();
  }
  writeln(getCommDiagnostics());
}

serialize(string, 4);
writeln();

serialize(string, 40);
writeln();

serialize(bytes, 4);
writeln();

serialize(bytes, 40);
writeln();

remoteMatch(string, 4);
writeln();

remoteMatch(string, 40);
writeln();

remoteMatch(bytes, 4);
writeln();

remoteMatch(bytes, 40);
writeln();