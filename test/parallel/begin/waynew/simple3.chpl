var a = 1;

begin {
  var b = 2;
  begin {
    var c:int;
    c = b * 2;
    writeln( "test1 - 2: c is ", c);
  }
  writeln( "test1 - 1: b is ", b);
}


begin {
  var d = 2;
  var e = 3;
  begin {
    var f:int;
    var g:int;
    f = d * 2;
    writeln( "test2 - 2: f is ", f);
    g = e * 2;
    writeln( "test2 - 2: g is ", g);
  }
  writeln( "test2 - 1: d is ", d);
  writeln( "test2 - 1: e is ", e);
}

