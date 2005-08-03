iterator basic() : integer {
  var a : integer;
  while a < 4 {
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

iterator easy() : integer {
  var c : integer;
  while c < 4 {
    yield c * 2;
    c += 2;
  }
}

iterator trivial() : integer {
  var d : integer;
  while d < 4 {
    yield d * 3;
    d += 2;
  }
}

function main() {
var x : integer = 5;
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