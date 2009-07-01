def main {
  var D = [1..10];
  var A: [i in D] int = i;

  def foo(X: [] int) {
    D = [1..10];
    writeln(X);
  }

  foo(A[D]);
}
