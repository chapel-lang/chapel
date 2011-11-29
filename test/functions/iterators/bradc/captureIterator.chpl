iter foo() {
  for i in 1..10 {
    yield i:real;
  }
}

var X = foo();

writeln("X is: ", X);
writeln("X.domain is: ", X.domain);
