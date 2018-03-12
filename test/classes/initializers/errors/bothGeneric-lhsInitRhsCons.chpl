// Verify behavior both the type with an initializer and
// the type with a constructor are generic
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


var hi: hasInit(int) = new hasConstruct(10);
writeln(hi);
delete hi;
