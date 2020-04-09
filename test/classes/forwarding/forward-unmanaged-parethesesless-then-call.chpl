class Inner {
  var element: int;
  proc this(x:int) {
    return element + x;
  }
}

class Impl {
  var theData:unmanaged Inner;
  proc data ref {
    return theData;
  }
}

record Thingy {
  forwarding var impl:unmanaged Impl;
}

proc test() {
  var x = new Thingy(new unmanaged Impl(new unmanaged Inner(1)));
  writeln(x);
  writeln(x.impl.data);
  writeln(x.impl.data(2));
  writeln(x.data);
  writeln(x.data(3));
  // x.data
  delete x.impl.theData;
  delete x.impl;
}

test();
