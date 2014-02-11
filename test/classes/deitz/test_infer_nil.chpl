class C {
  var x: int;
}

var c = nil;
c = new C();
writeln(c);
compilerWarning(typeToString(c.type));
writeln(c.x);
