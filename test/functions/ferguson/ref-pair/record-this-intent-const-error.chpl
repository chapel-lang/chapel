

record R {
  var field:int;

  proc setsField() {
    field = 1;
  }
}

const globalR = new R();

globalR.setsField();


