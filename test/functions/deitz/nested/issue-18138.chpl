proc foo(multiplier) {
  /* works

  forall i in 1..2 {
    multiplier.multiply(i);
  }

  */

  cobegin { // doesn't work
    multiplier.multiply(2);
    multiplier.multiply(1);
  }
}

proc main() {
  var a = [1,2,3];

  record Multiplier {
    proc multiply(i: int) {
      writeln(a*i);
    }
  }

  foo(new Multiplier());
}
