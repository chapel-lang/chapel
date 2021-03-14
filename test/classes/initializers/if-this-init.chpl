// Generate an error message for an unbalanced this.init()

record MyRec {
  var f0 : int;
  var f1 : int = 1;
  var f2 : int = 2;
  var f3 : int = 3;
  var f4 : int = 4;

  proc init(x : int) {
    if (x > 10) then
      this.init();
  }
}

proc main() {
  var r : MyRec = new MyRec(10);

  writeln(r);
}
