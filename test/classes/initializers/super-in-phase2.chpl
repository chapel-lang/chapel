record MyRec {
  var x : int = 0;
  var y : int = 1;

  proc init() {
    init this;
    super.init();
  }
}


proc main() {
  var r : MyRec;
}
