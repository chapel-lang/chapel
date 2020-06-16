module Outer {
  module A {
    record R {
      var x = 4;
    }
  }

  module User {
    use Outer;

    proc main() {
      forall i in 0..3 {
        var r = new A.R(i);
        // Not really interesting, but it means I don't have to sort the output
        // to test explicitly naming a record in a forall loop
        writeln(r.x == i);
      }
    }
  }
}
