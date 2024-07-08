class C {
  var x: int;
}

var c = nil;
c = (new owned C()).borrow();
writeln(c);
compilerWarning(c.type:string);
writeln(c.x);
