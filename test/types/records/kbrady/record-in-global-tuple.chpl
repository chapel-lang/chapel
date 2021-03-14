module bar {
  record BadBaz {
    var x = [1];

    proc init(y) where y.type != BadBaz {
      this.complete();
      x[0] = y;
    }

    proc init=(other: BadBaz) {
      this.complete();
      x[0] = other.x[0];
    }
  }

  var tuple_failure = (new BadBaz(2367),);
}

module foo {
  use bar;

  proc main() {
    writeln(tuple_failure[0].x[0]);
  }
}
