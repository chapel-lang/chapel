config param n = 5;

//
// This test generates n distinct versions of foo(), as shown
// by "a" having multiple types. Should we really allow this?
//
proc main {
  for param i in 1..n {
    proc foo() {
      var a: i*int;
      writeln("foo number ", i, " says a is: ", a);
      return a;
    }
    var a = foo();
  }
}
