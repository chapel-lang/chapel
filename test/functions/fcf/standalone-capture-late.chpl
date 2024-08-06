module A {
  module B {
    proc myFun() {
      writeln("executing inner myFun()");
    }
  }
  use B;

  proc myFun() {
    writeln("executing outer myFun()");
  }

  proc main() {
    myFun;   // expecting to print "executing myFun()"
  }
}
