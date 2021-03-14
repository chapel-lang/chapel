record Held {
  var x: int;

  proc init() {
    x = 10;
  }
}

class Holder {
  var h: Held;
}

var holder = new unmanaged Holder();
writeln(holder);
delete holder;
