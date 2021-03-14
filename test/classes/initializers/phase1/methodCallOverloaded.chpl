class Foo {
  var x: bool;

  proc init(val: int) {
    var xVal: bool;
    if (overloaded(val) > overloaded(val, val)) {
      xVal = false;
    } else {
      xVal = true;
    }
    x = xVal;
  }

  proc overloaded(val: int) {
    return val;
  }

  proc overloaded(val1: int, val2: int) {
    return val1 + val2;
  }
}

var foo = new unmanaged Foo(-1);
writeln(foo);
delete foo;
