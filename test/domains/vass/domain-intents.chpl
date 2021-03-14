writeln("2-blank");
var d2 = {11..111};
writeln("2a ", d2);
x2(d2);
writeln("2d ", d2);
proc x2(ref a2) { // note, now same as x6
  writeln("2b ", a2);
  a2 = {1111..11111};
  writeln("2c ", a2);
}

writeln("3-in");
var d3 = {11..111};
writeln("3a ", d3);
x3(d3);
writeln("3d ", d3);
proc x3(in a3) {
  writeln("3b ", a3);
  a3 = {1111..11111};
  writeln("3c ", a3);
}

writeln("4-out");
var d4 = {11..111};
writeln("4a ", d4);
x4(d4);
writeln("4c ", d4);
proc x4(out a4) {
  a4 = {1111..11111};
  writeln("4b ", a4);
}

writeln("5-inout");
var d5 = {11..111};
writeln("5a ", d5);
x5(d5);
writeln("5d ", d5);
proc x5(inout a5) {
  writeln("5b ", a5);
  a5 = {1111..11111};
  writeln("5c ", a5);
}

writeln("6-ref");
var d6 = {11..111};
writeln("6a ", d6);
x6(d6);
writeln("6d ", d6);
proc x6(ref a6) {
  writeln("6b ", a6);
  a6 = {1111..11111};
  writeln("6c ", a6);
}
