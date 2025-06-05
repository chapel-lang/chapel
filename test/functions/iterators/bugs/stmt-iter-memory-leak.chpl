class C {
  proc deinit() {
    writeln("deinit C");
  }
}

record R {
  proc deinit() {
    writeln("deinit R");
  }
}

iter foo() {
  yield new C();
  yield new C();
}

iter bar() {
  yield new R();
  yield new R();
}

foo();
bar();
