inline iter basic() : int {
  var a : int;
  while a < 4 {
    yield a * 4;
    a += 2;
  }
}

inline iter simple() : int {
  var b : int;
  while b < 6 {
    yield b * 10;
    b += 2;
  }
}

inline iter easy() : int {
  var c : int;
  while c < 4 {
    yield c * 2;
    c += 2;
  }
}

inline iter trivial() : int {
  var d : int;
  while d < 4 {
    yield d * 3;
    d += 2;
  }
}

proc main() {
var x : int = 5;
  for i in basic() {
    for j in simple() {
      for k in easy() {
        for m in trivial() {
          writeln(m + k + j + i + x);
        }
      }
    } 
  } 
}
