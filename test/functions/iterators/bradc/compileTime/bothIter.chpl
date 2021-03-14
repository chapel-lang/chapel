proc intSizes() param {
  yield 8;
  yield 16;
  yield 32;
  yield 64;
}
  

proc intTypes() type {
  for param z in intSizes() do
    yield int(z);
}

for type t in intTypes() {
  writeln("t is: ", t:string);
}
