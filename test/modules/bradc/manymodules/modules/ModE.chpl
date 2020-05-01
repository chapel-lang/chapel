module ModE {
  proc runea() {
    writeln("in runea() from module e");
  }

  module e2 {
    proc runea() {
      writeln("in runea() from module e2");
    }

    proc runeb() {
      writeln("in runeb() from module e2");
    }
  }


  proc runeb() {
    writeln("in runeb() from module e");
  }
}
