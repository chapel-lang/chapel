class Outer {

  var field:unmanaged Inner = nil;

  class Inner {
    var x:int;
  }

  proc init() {
    field = new unmanaged Inner(1);
  }
}

var x = new owned Outer();
writeln(x);
