module explicitName {
  module M {
    @edition(last="2.0")
    proc foo() {
      compilerWarning("old");
    }
    @edition(first="preview")
    proc foo() {
      compilerWarning("new");
    }
  }

  proc test1() {
    import this.M;
    M.foo();
  }
  proc test2() {
    use M;
    foo();
  }
  proc test3() {
    use M;
    M.foo();
  }
  test1();
  test2();
  test3();

}
