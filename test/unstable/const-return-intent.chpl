// 'const' return and yield intents are unstable, see:
// https://github.com/chapel-lang/chapel/issues/21888#issuecomment-1563535855


proc f1(len) {
  var a: [1..len] int;
  return a;
}

proc f2() const {
  return f1(4);
}

iter i3() const {
  for size in 1..3 do
    yield f1(size);
}

writeln(f1(2));
writeln(f2());
for arr in i3() do
  writeln(arr);
