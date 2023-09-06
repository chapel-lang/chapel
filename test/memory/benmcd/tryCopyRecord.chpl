record r {
  var i: int = 3;
}

var A: [0..10] r;

var B = A.tryCopy();

writeln(A);
writeln(B);
