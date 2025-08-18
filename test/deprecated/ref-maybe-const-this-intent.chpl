record R {
  var x: int = 11;
  var y: int = 12;
  proc foo() {
    x = 10;
  }
  proc bar() { // should not warn
    writeln((x,y));
  }
}
var r = new R();
r.foo();
r.bar();


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
sync r2.bar();
writeln(r2);
