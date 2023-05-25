
//
// This test exists to ensure that we can read in an unknown number of
// DefaultRectangular arrays using `channel.read`.
//
// Historical context:
// During the deprecation of the <~> operator, a trivial replacement of the
// operator in DefaultRectangular with `f.read(....)` resulted in an infinite
// loop in a couple of tests. The cause of this loop was that <~> threw on EOF
// and `channel.read` does not. The implementation at the time relied on an EOF
// error being thrown to stop trying to read.
//
// The other tests were exercising behavior unrelated to the reading of arrays,
// so this test was created.
//

use IO;

iter arrayReader(ch) {
  var A : [1..10] int;
  while ch.read(A) {
    yield A;
  }
}

proc main() {
  for A in arrayReader(stdin) {
    writeln("Read: ", A);
  }
}
