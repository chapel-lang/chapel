proc accessArg(ref a: int) var {
  return a;
}

var i: int;
accessArg(i) = 1;

writeln((accessArg(i), i));

var A: [1..5] int;
accessArg(A) = 2;

writeln((accessArg(A), A));
