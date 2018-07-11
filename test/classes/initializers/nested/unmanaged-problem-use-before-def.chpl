class Outer {

  var field:unmanaged Inner = nil;

  pragma "use default init"
  class Inner {
    var x:int;
  }

  proc init() {
    field = new unmanaged Inner(1);
  }
}

var x = new Outer();
writeln(x);
