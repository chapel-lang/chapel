use Reflection;
var x = ..6;
var y = ..7;
writeln(canResolve(">",x, y));

proc f() {
  compilerError("I am an error");
}

writeln(canResolve("f"));
