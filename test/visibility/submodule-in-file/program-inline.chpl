module Module {
  public module SubModule {
    public module SubSubModule {
      writeln("SubSubModule init");
      var subSubVar = 1;
      proc subSubFn() { return 2; }
    }

    writeln("SubModule init");
    var subVar = 10;
    proc subFn() { return 20; }
  }

  writeln("Module init");
  var v = 100;
  proc f() { return 200; }
}

module Program {
  import Module;

  proc main() {
    writeln(Module.v);
    writeln(Module.f());

    writeln(Module.SubModule.subVar);
    writeln(Module.SubModule.subFn());

    writeln(Module.SubModule.SubSubModule.subSubVar);
    writeln(Module.SubModule.SubSubModule.subSubFn());
  }
}
