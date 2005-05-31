class foo {
  var x : integer = 12;
  constructor initialize(x : integer) {
    this.x = x + 10;
  }
}

var f : foo = foo(50);

writeln(f);
