record Foo {
  var funcField: func(int, bool);

  proc callTheField(arg: int) {
    if (funcField(arg)) {
      writeln("fcf with arg ", arg, " successful");
    } else {
      writeln("fcf with arg ", arg, " unsuccessful");
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

var f = new Foo(bar);
f.callTheField(2);
f.callTheField(6);
