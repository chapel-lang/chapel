config const i = 1;
config const j = 1;
config const n = 2;
config const makesame = false;

class C {
  var A:[1..n] int;
}

proc main() {
  var cOwn = new owned C();
  var c = cOwn.borrow();
  var dOwn = new owned C();
  var d = dOwn.borrow();

  if makesame {
    d = c;
  }

  // Now we cannot conclude that c doesn't alias d
  var sum = 0;
  for k in 1..n {
    sum += c.A[i] + d.A[j];
    c.A[i] += 1;
  }
  writeln(sum);

  sum = arrayArgs(c.A, d.A);
  writeln(sum);

  sum = classArgs(c, d);
  writeln(sum);
}

proc arrayArgs(ref a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a[j] + b[j];
    a[j] += 1;
  }
  return sum;
}

proc innerArrayArgs(ref a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a[j] + b[j];
    a[j] += 1;
  }
  return sum;
}

proc classArgs(c, d) {
  return innerArrayArgs(c.A, d.A);
}


