use CommDiagnostics;

config param foo = false;

class Helper {
  var x : 3*int;
}

record Foo {
  var h : unmanaged Helper?;
  var serialized = false;

  proc chpl__serialize() {
    return h.x;
  }

  proc type chpl__deserialize(data) where foo {
    var f : Foo;
    f.h = new unmanaged Helper(data);
    f.serialized = true;
    return f;
  }

  proc deinit() {
    delete h;
  }
}

proc main() {
  const ff = new Foo(new unmanaged Helper((1,2,3)));

  startCommDiagnostics();
  on Locales.tail() {
    if ff.h.x(1) == 100 then halt();
  }
  stopCommDiagnostics();

  for (loc, dat) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", dat);
  }
}
