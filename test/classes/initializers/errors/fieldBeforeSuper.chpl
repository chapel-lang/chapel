
class C {
  var x : int;

  proc init() {
    this.x = 5;
    super.init();
  }
}

var c = new borrowed C();
