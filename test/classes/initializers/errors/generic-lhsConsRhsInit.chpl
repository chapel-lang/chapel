// Verify behavior when the type with a constructor is generic,
// but the type with an initializer is concrete
class hasInit {
  var x: int;

  proc init(xVal) {
    x = xVal;
  }
}

class hasConstruct {
  var x;

  proc hasConstruct(x) {
    this.x = x;
  }
}


var hi: borrowed hasConstruct(int) = new borrowed hasInit(10);
writeln(hi);
