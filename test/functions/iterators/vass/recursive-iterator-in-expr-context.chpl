iter try1() {
  yield 5;
  try1(); // recurse+discard
}

iter try2(col:int) {
  if col <= 0 then yield col;
  else try2(col - 1);  // recurse+discard
}

iter try3():int {
  var a = try3(); // recurse (infinitely), do not discard
  yield a(1);
}

proc main() {
  for i in try1()  do writeln(i);
  for i in try2(2) do writeln(i);
  for i in try3()  do writeln(i);
}
