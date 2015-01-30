config const unsure = true;

export proc foo(bar: real, baz: real) {
  proc socket(connect) {
    var socket = 2.2 + connect;
    writeln("In foo().socket(), local var socket is: ", socket);
  }
  writeln("In foo, argument baz is ", baz);
  socket(bar);
}

/* Can't export types yet  :(

export record bar {
  var foo: real = 1.2;
  proc baz() {
    var socket = 3.4;
    writeln("In bar.baz, socket is: ", socket);
  }
}
*/

//
// Can't export vars yet  :(
//
// export var baz: bar;

foo(1.2, 5.7);
/*
writeln(baz);
writeln(baz.foo);
baz.socket();
*/
