

record R {
  var field:int;

  proc ref setsField() {
    field = 1;
  }
}

const globalR = new R();

globalR.setsField();


