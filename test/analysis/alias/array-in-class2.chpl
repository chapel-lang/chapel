config const i = 1;
config const j = 1;
config const n = 2;
config const makesame = false;

class C {
  var A:[1..n] int;
}

proc main() {
  var d = new borrowed C();

  var A:[1..n] int; 
  // Now we cannot conclude that c doesn't alias d
  var sum = 0; 
  for k in 1..n {
    sum += A[i] + d.A[j];
    A[i] += 1;
  }
  writeln(sum);

  sum = arrayArgs(A, d.A);
  writeln(sum);

  sum = classArgs(A, d);
  writeln(sum);
}

proc arrayArgs(a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a[j] + b[j];
    a[j] += 1;
  }
  return sum;
}

proc innerArrayArgs(a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a[j] + b[j];
    a[j] += 1;
  }
  return sum;
}

proc classArgs(A, d) {
  return innerArrayArgs(A, d.A);
}


