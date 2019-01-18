record MyRec {
  var x : int;
  var y : int = 1;
  var z : int = 2;

  proc init() {
    writeln('init()     Phase 0');
    writeln();

    this.init(5);

    writeln();
    writeln('init()     Phase 2');
  }

  proc init(_z : int) {
    writeln('init(_z)   Phase 1');

    z = _z;


    writeln('init(_z)   Phase 2');
  }
}

proc main() {
  var r1 : MyRec;

  writeln();
  writeln('r1: ', r1);
}
