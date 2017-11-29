proc foo(ref a:int, ref b:int = a) {
  a = 10;
  writeln(b);
}

var x:int = 1;
foo(x);
