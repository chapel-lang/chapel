record R {
  var x: int;
  @edition(last="2.0")
  proc init(x: int) {
    compilerWarning("old");
    this.x = x;
  }
  @edition(first="preview")
  proc init(x: int) {
    compilerWarning("new");
    this.x = x;
  }

  @edition(last="2.0")
  proc foo() {
    compilerWarning("old");
  }
  @edition(first="preview")
  proc foo() {
    compilerWarning("new");
  }

  @edition(last="2.0")
  proc type bar() {
    compilerWarning("old");
  }
  @edition(first="preview")
  proc type bar() {
    compilerWarning("new");
  }
}

var r = new R(17);
r.foo();
R.bar();
