record Foo {
  var b: shared Bar?;
  forwarding b!;

  proc init() {
    b = new shared Bar();
  }
}

class Bar {
  proc const whatever() {
    return 16;
  }
}

proc main() {
  var f = new Foo();
  writeln(f.whatever());
}
