// Confirm that the compiler accepts multi-arm conditional with init in
// all branches

record MyRec {
  var x : int = 10;
  var y : int = 20;

  proc init() {
    writeln('  Init()');
    x = 20;
    y = 30;
  }

  proc init(a : int) {
    writeln('Init(int) Enter');

    if a < 5 {
      writeln('          a <  5');
      this.complete();

    } else if a < 10 {
      writeln('          a < 10');
      this.complete();

    } else {
      this.init();
    }

    writeln('Init(int) Done');
    writeln();
  }
}

proc main() {
  var r1 : MyRec = new MyRec( 0);
  var r2 : MyRec = new MyRec( 5);
  var r3 : MyRec = new MyRec(15);

  writeln('r1: ', r1);
  writeln('r2: ', r2);
  writeln('r3: ', r3);
}
