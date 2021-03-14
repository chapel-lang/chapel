
{
  var a: [{1..1}] bytes;
  var b: [{1..1}] bytes;
  a[1] = b"OK";
  b=a;
  writeln(b);
}

{
  use List;
  var l: list(bytes);
  l.append(b"OK");
  var a = l.toArray();
  writeln(a);
}
