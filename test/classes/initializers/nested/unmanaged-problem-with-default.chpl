class Outer {

  var field:unmanaged Inner;

  class Inner {
    var x:int;
  }
}

var x = new Outer();
writeln(x);
