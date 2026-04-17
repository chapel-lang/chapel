module M1 {
  record R1 {}
  record R2 {}

  module M2 {
    import super.R1;
    import super.R2;
    proc R1.doSomething1() { writeln("R1.doSomething1 called"); }
    proc R2.doSomething2() { writeln("R2.doSomething2 called"); }
  }

  import this.M2.R1;
  import this.M2.{R2};

  proc main() {
    var r1: R1;
    r1.doSomething1();
    var r2: R2;
    r2.doSomething2();
  }
}
