record Held {
  var x: int;

  proc init() {
    x = 10;
  }
}

class Holder {
  var h: Held;
}

var holder = new Holder();
writeln(holder);
delete holder;
