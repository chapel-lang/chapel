module DefinesRec {
  record Foo {
    var x: int;

    operator +(lhs: Foo, rhs: Foo) {
      return new Foo(lhs.x + rhs.x);
    }
  }
}

module Main {
  proc returnRec(x: int) {
    use DefinesRec;
    return new Foo(x);
  }

  proc main() {
    var one = returnRec(3);
    var two = returnRec(4);
    writeln(one + two);
  }
}
