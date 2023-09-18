pragma "no doc"
module M {

  proc bar() {
    writeln("bar");
  }
}

module N {
  pragma "no doc"
  proc foo() {
    writeln("foo");
  }

  pragma "no doc"
  config const x = 1;

  pragma "no doc"
  record R {
    var x: int;
  }

  class C {
    pragma "no doc"
    var y: int;
  }

}

proc main() {
  writeln("in main");
}
