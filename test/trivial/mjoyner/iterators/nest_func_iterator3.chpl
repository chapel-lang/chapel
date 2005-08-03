iterator basic() : integer {
  var i : integer;
  while i < 10 {
    yield i * 4;
    i += 2;
  }
}

iterator simple() : integer {
  var i : integer;
  while i < 10 {
    yield i * 10;
    i += 2;
  }
}

function main() {
var x : integer = 5;
  for i in basic() {
    writeln(i+x);
  } 

  for j in simple() {
    writeln(j + x);
  }
}