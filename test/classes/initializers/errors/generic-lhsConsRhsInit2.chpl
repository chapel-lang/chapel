// Verify behavior when the type with a constructor is concrete,
// but the type with an initializer is generic
class hasInit {
  var x;

  proc init(xVal) {
    x = xVal;
  }
}

class hasConstruct {
  var x: int;

  proc hasConstruct(xVal) {
    x = xVal;
  }
}


var hi: hasConstruct = new hasInit(10);
writeln(hi);
delete hi;
