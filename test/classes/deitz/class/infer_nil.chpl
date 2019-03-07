class C {
  var x: int;
}

var c = nil;
c = new borrowed C();
writeln(c);
compilerWarning(c.type:string);
writeln(c.x);
