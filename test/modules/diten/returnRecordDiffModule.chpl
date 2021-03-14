module M1 {
  record R {
    var a = 2;
    proc foo() {
      return a;
    }
  }
}

module M2 {
  proc main {
    var r = retRecord();
    writeln(r.a);

    proc retRecord() {
      use M1;
      return new R();
    }
  }
}
