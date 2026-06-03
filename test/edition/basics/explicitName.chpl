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
  proc test4() {
    import this.M.foo;
    foo();
  }
  proc test5() {
    import this.M as M2;
    M2.foo();
  }
  proc test6() {
    import this.M.foo as foo2;
    foo2();
  }
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();

}
