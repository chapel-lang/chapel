use CommDiagnostics, Map;

config const n = 10;

class Helper {
  var x : 3*range;
}

record Foo {
  var h : unmanaged Helper;
  inline proc ranges {
    return h.x;
  }

  proc chpl__serialize() {
    return ranges;
  }

  proc type chpl__deserialize(data) {
    return new Foo(new unmanaged Helper(data));
  }

  proc deinit() {
    delete h;
  }
}

var commStats = new map(string, [LocaleSpace] commDiagnostics);

proc start() {
  startCommDiagnostics();
}

proc stop(msg : string) {
  stopCommDiagnostics();
  const stats = getCommDiagnostics();
  commStats[msg] = stats;
  resetCommDiagnostics();
}

proc main() {
  const R = (1..n-1, 1..n, 1..n+1);
  const f = new Foo(new unmanaged Helper(R));

  start();
  on Locales[numLocales-1] {
    const x = f.ranges;
    if x(1).low == 0 then halt("something");
  }
  stop("on-stmt");

  start();
  coforall loc in Locales do on loc {
    const x = f.ranges;
    if x(1).low == 0 then halt("something");
  }
  stop("coforall-on");

  start();
  sync {
    begin on Locales[numLocales-1] {
      const x = f.ranges;
      if x(1).low == 0 then halt("something");
    }
  }
  stop("begin-on");

  for (msg, dat) in commStats.items() {
    const sep = "===== " + msg + " =====";
    writeln(sep);
    for (loc, dat) in zip(Locales, dat) {
      writeln(loc, ": ", dat.get);
    }

    writeln("=" * sep.size);
    writeln();
  }
}
