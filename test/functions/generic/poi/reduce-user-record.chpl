/*
A simplification of 
  test/classes/initializers/records/reductionOverCreatedArray-generalType.chpl
*/

record Foo {
  var x: int;
}

// We want these to be visible to the implementation of 'reduce'.
operator :(const x: int, type t: Foo) do  return new Foo(x);
operator *(a: Foo, b: Foo) do         return new Foo(a.x*b.x);

proc main() {
  writeln( * reduce for i in 1..2 do new Foo(i) );
}
