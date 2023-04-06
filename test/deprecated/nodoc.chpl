@chpldoc.nodoc
module M {

  proc bar() {
    writeln("bar");
  }
}

module N {
  @chpldoc.nodoc
  proc foo() {
    writeln("foo");
  }

  @chpldoc.nodoc
  config const x = 1;

  @chpldoc.nodoc
  record R {
    var x: int;
  }

  class C {
    @chpldoc.nodoc
    var y: int;
  }

}

proc main() {
  writeln("in main");
}
