pragma "use default init"
class Outer {

  var field:unmanaged Inner;

  pragma "use default init"
  class Inner {
    var x:int;
  }
}

var x = new Outer();
writeln(x);
