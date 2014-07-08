record Bar {
  var str: string;
}

iter foo(n:int, b:Bar) {
  b.str = "blah";
  for i in 1..n do 
    yield "foo";
  writeln(b.str);
}

var g = new Bar();
g.str = "bar";
for t in foo(10, g) do 
 writeln(t);
