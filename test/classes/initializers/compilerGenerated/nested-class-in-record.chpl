record enclosing {
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

var mine = new enclosing(5);
mine.checkInner();
