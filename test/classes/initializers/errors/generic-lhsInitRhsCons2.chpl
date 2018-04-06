// Verify behavior when the type with an initializer is concrete,
// but the type with a constructor is generic
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


var hi: hasInit = new hasConstruct(10);
writeln(hi);
delete hi;
