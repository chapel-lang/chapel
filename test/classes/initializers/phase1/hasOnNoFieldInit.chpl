class Foo {
  var x: int;

  proc init(xVal) {
    var remoteVal: xVal.type;
    on xVal {
      remoteVal = xVal;
    }
    x = remoteVal;
    super.init();
  }
}

var foo = new Foo(3);
writeln(foo);
delete foo;
