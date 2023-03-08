// Tracks a bug I found when using throwing initializers in practice
module tryBangExpression {
  proc validate(xVal: int) throws {
    if xVal > 10 then
      throw new Error("value too large");
  }

  class Foo {
    var x: int;

    proc init(xVal: int) throws {
      x = xVal;
      this.complete();
      validate(xVal);
    }
  }

  proc main() {
    // Using a try! expression instead of a try! statement caused problems
    var x: Foo = try! new Foo(8);
    writeln(x);
    var y: Foo = try! new Foo(15);
    writeln(y);
  }
}
