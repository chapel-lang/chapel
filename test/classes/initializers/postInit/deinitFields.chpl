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

  proc init() throws {
    writeln("In C's init");
  }

  proc postinit() throws {
    throw new Error();
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
