record Bar {
  var str: string;
}

iter foo(n:int, b:Bar) {
  var gg = set(b);
  for i in 1..n do 
    yield "foo";
  writeln(gg.str);
}

proc set(b:Bar) {
  b.str = "blah";
  return b;
}

var g = new Bar();
g.str = "bar";
for t in foo(10, g) do 
 writeln(t);
