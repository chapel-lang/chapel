iter I(i...?k) {
  for param d in 0..<k {
    for b in i(d) {
      yield b;
    }
  }
}

var A = [0, 1, 2, 3, 4];
var res = (I(A) == 2);
writeln(res);
