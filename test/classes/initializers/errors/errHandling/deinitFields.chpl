record R {
  proc init() {
    writeln("In R's init");
  }

  proc deinit() {
    writeln("In R's deinit");
  }
}

class C {
  var r: R;

  proc callThrowing() throws {
    throw new Error();
  }

  proc init() throws {
    writeln("In C's init");
    init this;
    callThrowing();
  }

  proc deinit() {
    writeln("In C's deinit");
  }
}

try {
  var myC = new C();
} catch {
  writeln("Caught error");
}
writeln("Past creation of myC");
