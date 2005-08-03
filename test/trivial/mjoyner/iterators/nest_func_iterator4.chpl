iterator basic() : integer {
  var a : integer;
  while a < 10 {
    yield a * 4;
    a += 2;
  }
}

iterator simple() : integer {
  var b : integer;
  while b < 6 {
    yield b * 10;
    b += 2;
  }
}

function main() {
var x : integer = 5;
  for i in basic() {
    for j in simple() {
      writeln(j + i + x);
    } 
  } 
}