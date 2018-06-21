class enclosing {
  var a: int;

  class inner {
    var b: bool;
  }

  proc checkInner() {
    var myInner = new unmanaged inner(true);
    writeln(myInner);
    delete myInner;
  }
}

var mine = new unmanaged enclosing(5);
mine.checkInner();
delete mine;
