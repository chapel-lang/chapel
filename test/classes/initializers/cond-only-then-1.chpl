// Confirm that the compiler rejects a conditional with an init in the
// consequent but no init in the alternative

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

    if a < 10 then
      this.complete();
    else
      writeln('Else does not contain an init');

    writeln('Init(int) Done');
    writeln();
  }
}

proc main() {
  var r1 : MyRec = new MyRec( 5);
  var r2 : MyRec = new MyRec(15);

  writeln('r1: ', r1);
  writeln('r2: ', r2);
}
