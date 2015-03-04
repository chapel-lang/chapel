// This test verifies that a type which does not allow noinit will
// not generate an error or warning message when compiled with
// --no-use-noinit, but will when compiled with --use-noinit or
// no flag thrown (because --use-noinit is the default behavior)

// Lydia note 1: The pragma used here is temporary.  When the
// implementation of noinit is finished, the absence of a noinit
// constructor will be used instead of the pragma.
pragma "ignore noinit"
record R {
  var a: int;

  proc R() {
    writeln("default init!");
    a = 4;
  }
}

var foo: R;
var bar: R = noinit;
// Lydia note 2: in our constructor deep dive, it was decided that
// this case should error out instead of replacing noinit with default
// initialization and generating a compiler warning.  However, this
// has not been implemented yet (and should be in a separate commit
// anyways), so for now the expected output of this test includes a
// warning and the result of executing the compiled program.
