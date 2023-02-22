// Tracks a bug I found when using throwing initializers in practice
module tryBangExpressionDefault {
  proc validate(xVal: int) throws {
    if xVal > 10 then
      throw new Error("value too large");
  }
  record Bar {
    var x: int;

    proc type a { return new Bar(14); }
    proc type b { return new Bar(11); }
  }

  record Foo {
    var x: Bar;

    // Using a default value that was slightly complicated, e.g. the result of a
    // type method call triggered a specific error message
    proc init(xVal: Bar = Bar.a) throws {
      x = xVal;
      this.complete();
      validate(xVal.x);
    }
  }

  proc main() {
    // Using a try! expression instead of a try! statement also caused problems
    var x: Foo = try! new Foo();
    writeln(x);
  }
}
