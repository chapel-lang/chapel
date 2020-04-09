class Foo {
  var x: int;

  proc init(xVal) {
    var remoteVal: xVal.type;
    on xVal {
      remoteVal = xVal;
    }
    x = remoteVal;
  }
}

var foo = new owned Foo(3);
writeln(foo);
