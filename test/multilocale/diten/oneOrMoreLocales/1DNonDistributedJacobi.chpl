config const size = 10;
config const epsilon = 0.0001;

proc main {
  var A: [0..size-1] real;
  var B: [0..size-1] real;
  var delta: real;

  A(0) = 1.0;

  do {
    delta = 0.0;
    for i in 0..size-1 {
      if (i == 0) {
        B(i) = 1.0;
      } else if (i == size-1) {
        B(i) = A(i-1)/2.0;
      } else {
        B(i) = (A(i-1)+A(i+1))/2.0;
      }
      delta = max(delta, abs(B(i) - A(i)));
    }
    A = B;
  } while (delta > epsilon);
  for i in 0..size-1 {
    writeln(A(i));
  }
}

