def basic() : int {
  var a : int;
  while a < 10 {
    yield a * 4;
    a += 2;
  }
}

def simple() : int {
  var b : int;
  while b < 6 {
    yield b * 10;
    b += 2;
  }
}

def main() {
var x : int = 5;
  for i in basic() {
    for j in simple() {
      writeln(j + i + x);
    } 
  } 
}
