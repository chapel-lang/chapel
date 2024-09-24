record R {
  var x : int = 13;
}

var foo: [0:uint(8)..255:uint(8)] R;

writeln(foo[42:uint(8)]);
