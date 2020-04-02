module M {

  module SubModule {
    var x:int;
    proc f() { return 1; }

    module SubSubModule {
      proc foo() {
        use super.super.DoesNotExistModule; // error
        writeln(SubModule.x);
        writeln(SubModule.f());
      }
    }
    public import this.SubSubModule;
  }

  proc main() {
    import this.SubModuleSubSubModule;
    SubSubModule.foo();
  }
}
