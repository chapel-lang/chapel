proc f(x) type {
  select x.type {
    when int(32) do return real(32);
    when int(64) do return real(64);
    otherwise return real(32);
  }
}

var a: f(1:int(64));
var b: f(1:int(32));
var c: f(1:int(16));
writeln(typeToString(a.type));
writeln(typeToString(b.type));
writeln(typeToString(c.type));
