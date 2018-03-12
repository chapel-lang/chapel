// Verify behavior when both the type with a constructor and
// the type with an initializer are generic
class hasInit {
  var x;

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


var hi: hasConstruct(int) = new hasInit(10);
writeln(hi);
delete hi;
