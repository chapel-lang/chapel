record Foo {
  type funcType;
  var funcField: funcType;

  proc init(y: proc(x: int): bool) {
    funcType = proc(x: int): bool;
    funcField = y;
  }

  proc init() {
    funcType = nothing;
    funcField = none;
  }

  proc callTheField(arg: int) {
    if (funcType != nothing) {
      if (funcField(arg)) {
        writeln("fcf with arg ", arg, " successful");
      } else {
        writeln("fcf with arg ", arg, " unsuccessful");
      }
    } else {
      writeln("no fcf stored");
    }
  }
}

proc bar(x: int): bool {
  if (x * 3 > 15) {
    return true;
  } else {
    return false;
  }
}

var f1 = new Foo(bar);
f1.callTheField(2);
f1.callTheField(6);
var f2 = new Foo();
f2.callTheField(17);
