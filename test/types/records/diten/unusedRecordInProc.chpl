proc foo() {
  record R {
    var a: int;
    var b: int;
  }

  var r = new R();
}

writeln("Not calling foo.");
// foo();
