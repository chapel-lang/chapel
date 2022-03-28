use Map;

proc int.hash() {
  writeln("int.hash() shouldn't be called");
}

proc hash(i: int) {
  writeln("hash(int) shouldn't be called");
}

var m = new map(int, int);

m[0] = 5;
writeln("value stored: ", m[0]);
