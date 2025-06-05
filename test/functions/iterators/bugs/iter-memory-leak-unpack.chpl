class C {
  var id: int;
  proc deinit() {
    writeln("deinit C id=", id);
  }
}

record R {
  var id: int;
  proc deinit() {
    writeln("deinit R id=", id);
  }
}

iter foo() {
  yield (new C(1), new C(2));
  yield (new C(3), new C(4));
}

iter bar() {
  yield (new R(1), new R(2));
  yield (new R(3), new R(4));
}


for (_, x) in foo() {
  writeln(x);
}
for (_, x) in bar() {
  writeln(x);
}
