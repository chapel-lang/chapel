record R {
  var desc: string;
  proc deinit() { writeln("R: ", this); }
}
class C {
  proc deinit() { writeln("C: ", this); }
}

proc foo() {
  var rOuter1 = new R("Outer 1");
  defer {
    var rInner = new R("Inner 1");
  }
  var rOuter2 = new R("Outer 2");
  defer {
    var rInner = new R("Inner 2");
  }
  var rOuter3 = new R("Outer 3");
}

writeln("Start foo");
foo();
writeln("End foo");
writeln();

proc bar() {
  var rOuter1 = new R("Outer 1");
  defer {
    var rInner1 = new R("Inner 1");
    defer {
      var rInnerInner = new R("InnerInner 1");
    }
    var rInner2 = new R("Inner 2");
  }
  var rOuter2 = new R("Outer 2");
}

writeln("Start bar");
bar();
writeln("End bar");
writeln();

proc baz() {
  var c = new unmanaged C();
  var rOuter1 = new R("Outer 1");
  defer {
    var rInner1 = new R("Inner 1");
    delete c;
    defer {
      var rInnerInner = new R("InnerInner 1");
    }
    var rInner2 = new R("Inner 2");
  }
  var rOuter2 = new R("Outer 2");
}

writeln("Start baz");
baz();
writeln("End baz");
writeln();

proc bat() throws {
  var c = new unmanaged C();
  var rOuter1 = new R("Outer 1");
  defer {
    var rInner1 = new R("Inner 1");
    delete c;
    throw new Error();
    defer {
      var rInnerInner = new R("InnerInner 1");
    }
    var rInner2 = new R("Inner 2");
  }
  var rOuter2 = new R("Outer 2");
}

writeln("Start bat");
try {
  bat();
}
catch {
  writeln("Caught error from bat");
}
writeln("End bat");
writeln();
