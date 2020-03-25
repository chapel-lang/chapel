module OuterModule {
  module a {
    var x = 2;
    writeln("a");
    module b { import OuterModule.a.x; var aaa = x; writeln("a.b ", aaa); }
  }

  module b { writeln("b"); }

  proc main() {
    use a.b;
    foo();
  }

  proc foo() {
    use b;
  }
}
