iterator basic() : int {
  var a : int;
  while a < 10 {
    yield a * 4;
    a += 2;
  }
}

iterator simple() : int {
  var b : int;
  while b < 6 {
    yield b * 10;
    b += 2;
  }
}

function main() {
var x : int = 5;
  for i in basic() {
    for j in simple() {
      writeln(j + i + x);
    } 
  } 
}
