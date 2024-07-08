{
  var x = (-18.0, inf);
  writeln(x, ": ", x.type:string);
  var y = x: complex(128);
  writeln(y, ": ", y.type:string);
  writeln((y.re, y.im));
  writeln();
}

{
  var x = (-18.0:real(32), inf:real(32));
  writeln(x, ": ", x.type:string);
  var y = x: complex(64);
  writeln(y, ": ", y.type:string);
  writeln((y.re, y.im));
  writeln();
}

{
  var x = (-18.0:real(32), inf:real(32));
  writeln(x, ": ", x.type:string);
  var y = x: complex(128);
  writeln(y, ": ", y.type:string);
  writeln((y.re, y.im));
  writeln();
}

