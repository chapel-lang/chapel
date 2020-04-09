proc intTypes() type {
  yield int(8);
  yield int(16);
  yield int(32);
  yield int(64);
}
  

for type t in intTypes() {
  writeln("t is: ", t:string);
}
