module M {

  module SubModule {
    var x:int;
    proc f() { return 1; }

    module SubSubModule {
      proc foo() {
        use super.super.SubModule;
        writeln(SubModule.x);
        writeln(SubModule.f());
      }
    }
    public import this.SubSubModule;
  }

  proc main() {
    use this.SubModule;
    writeln(SubModule.x);
    writeln(SubModule.f());

    SubSubModule.foo();
  }
}
