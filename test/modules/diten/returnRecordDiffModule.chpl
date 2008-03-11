module M1 {
  record R {
    var a = 2;
    def foo() {
      return a;
    }
  }
}

module M2 {
  def main {
    var r = retRecord();
    //writeln(r.a);

    def retRecord() {
      use M1;
      return new R();
    }
  }
}
