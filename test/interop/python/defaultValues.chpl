export proc cstringDefault(in x: c_string = "blah") {
  writeln(createStringWithNewBuffer(x));
}

export proc intDefault(x: int = 3) {
  writeln(x);
}

export proc realDefault(x: real = 2.5) {
  writeln(x);
}

export proc boolDefault(x: bool = true) {
  writeln(x);
}

export proc uintDefault(x: uint = 7) {
  writeln(x);
}
/* Can't use this due to #11530
export proc arrayDefault(x: [0..2] int = [1, 2, 3]) {
  writeln(x);
}
*/
