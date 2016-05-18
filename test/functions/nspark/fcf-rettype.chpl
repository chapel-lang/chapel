{
  var F = lambda (x: int) { writeln(x); return x + 42; };
  writeln(F.retType:string);
  writeln(F(13));
  writeln();
}

{
  record R {
    var a: int;
    var b: real;
  }
  var F = lambda () {
    var r = new R(13, 42.0);
    writeln(r);
    return r;
  };
  writeln(F.retType:string);
  writeln(F());
  writeln();
}

{
  record R {
    type eltType;
    var a: eltType;
    var b: eltType;
  }
  var F = lambda () {
    var r = new R(real, 13.0, 42.0);
    writeln(r);
    return r;
  };
  writeln(F.retType:string);
  writeln(F());
  writeln();
}

{
  var F = lambda (x: real) { writeln(x); return x:string; };
  writeln(F.retType:string);
  writeln(F(42));
  writeln();
}
