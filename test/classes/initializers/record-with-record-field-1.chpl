record MyRec {
  var x : int = 10;
  var y : SubRec;

  proc init() {
    writeln('MyRec.init()');
  }
}

record SubRec {
  var a : int = 10;
  var b : int = 20;

  proc init() {
    writeln('SubRec.init()');
  }
}

proc main() {
  var r : MyRec;

  writeln('r: ', r);
}