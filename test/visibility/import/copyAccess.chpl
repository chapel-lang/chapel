module RecordProvider {
  record Foo {
    var x = 3;
  }
  proc Foo.init=(ref other: Foo) {
    writeln("In Foo.init=");
    this.x = other.x;
  }

  proc Foo.deinit() {
    writeln("In Foo.deinit");
  }
}

module User {
  import RecordProvider;

  proc main() {
    var a = new RecordProvider.Foo();
    var b = copyTrigger(a);
    writeln(b.x);
  }

  proc copyTrigger(const ref x) {
    return x;
  }
}
