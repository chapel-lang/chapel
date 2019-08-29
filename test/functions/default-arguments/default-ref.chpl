proc foo(ref a:int, ref b:int = a) {
  a = 10;
  writeln(b);
}

proc refidentity(ref arg) ref
{
  return arg;
}

proc bar(ref a:int, ref b = refidentity(a)) {
  a = 11;
  writeln(b);
}

var global: int = 0;
proc baz(ref a:int, ref b = global) {
  global = 12;
  writeln(b);
}

var x:int = 1;
foo(x);
bar(x);
baz(x);

proc onemore() {
  var y:int = 2;
  var outervar: int = 0;
  proc boo(ref a:int, ref b = outervar) {
    outervar = 13;
    writeln(b);
  }
  boo(y);
}
onemore();
