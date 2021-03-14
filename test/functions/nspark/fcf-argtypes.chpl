{
  var F = lambda (x: int) { return x + 42; };
  writeln("argTypes = ", F.argTypes:string);
  writeln("retType  = ", F.retType:string);
  writeln(F(13));
  writeln();
}

{
  record R {
    var a: int;
    var b: real;
  }
  var F = lambda (x: int, y: real) {
    var r = new R(x, y);
    return r;
  };
  writeln("argTypes = ", F.argTypes:string);
  writeln("retType  = ", F.retType:string);
  writeln(F(42, 13.0));
  writeln();
}

{
  proc foo(x: string, y: int) {
    var tmp = x + y:string;
    return tmp;
  }
  var F = foo;
  writeln("argTypes = ", F.argTypes:string);
  writeln("retType  = ", F.retType:string);
  writeln(foo("thirteen = ", 13));
  writeln();  
}
