record R2 {
  var x = 0;
  proc ref foo() {
    coforall l in Locales do on l {
      editThis();
    }
  }
  proc ref editThis() {
    x += 1;
  }

  proc ref bar() {
    begin on Locales[0] do editThis();
  }
}
var r2 = new R2();
r2.foo();
writeln(r2);
