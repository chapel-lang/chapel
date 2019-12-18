module ModD {
  module d2 {
    proc rund() {
      writeln("in rund() from module d2");
    }
  }


  proc rund() {
    writeln("in rund() from module d");
  }
}
