module choose {
  class C { var x: int; }
  proc choose(maybeNil: C?, notNil: C): C {
    var ret:C = notNil; // needs initializer
    if maybeNil != nil {
      ret = maybeNil!;
    }
    return ret;
  }

  proc main() {
    var one = new owned C(1);
    var two = new owned C(2);
    var n:C?;

    writeln(choose(n, one));
    writeln(choose(one, two));
  }
}
