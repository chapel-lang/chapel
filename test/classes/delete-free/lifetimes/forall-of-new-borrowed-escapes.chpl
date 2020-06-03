class C {
  var x: int;

  proc init(x) {
    this.x = x;
    writeln("Initing C");
  }
  proc deinit() {
    writeln("De-initing C");
  }
}

var x = [i in 1..3] new borrowed C(i);

writeln(x);
