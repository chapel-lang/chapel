module ModC {
  proc runc() {
    writeln("in runc() from module c");
  }


  module c2 {
    proc runc() {
      writeln("in runc() from module c2");
    }
  }
}
