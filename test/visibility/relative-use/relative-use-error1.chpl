module M {

  module SubModule {
    var x:int;
    proc f() { return 1; }

    module SubSubModule {
    }
    public import this.SubSubModule;
  }

  proc main() {
    import this.SubModule;
    use this.SubSubModule; // oops! does not exist at that path
    writeln(SubModule.x);
    writeln(SubModule.f());
  }
}
