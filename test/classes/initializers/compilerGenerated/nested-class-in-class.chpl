class enclosing {
  var a: int;

  class inner {
    var b: bool;
  }

  proc checkInner() {
    var myInner = new inner(true);
    writeln(myInner);
    delete myInner;
  }
}

var mine = new enclosing(5);
mine.checkInner();
delete mine;
