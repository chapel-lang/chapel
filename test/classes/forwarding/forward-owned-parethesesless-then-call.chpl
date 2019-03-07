class Inner {
  var element: int;
  proc this(x:int) {
    return element;
  }
}

class Impl {
  var theData:owned Inner;
  proc data ref {
    return theData;
  }
}

record Thingy {
  forwarding var impl:owned Impl;
}

proc test() {
  var x = new Thingy(new owned Impl(new owned Inner(1)));
  writeln(x);
  writeln(x.data);
  writeln(x.data(1));
  // x.data
}

test();
