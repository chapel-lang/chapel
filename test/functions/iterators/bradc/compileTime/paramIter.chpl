proc intSizes() param {
  yield 8;
  yield 16;
  yield 32;
  yield 64;
}
  

for type t in intSizes() {
  writeln("t is: ", int(t):string);
}
