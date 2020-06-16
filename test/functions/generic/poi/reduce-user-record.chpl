/*
A simplification of 
  test/classes/initializers/records/reductionOverCreatedArray-generalType.chpl
*/

record Foo {
  var x: int;
}

// We want these to be visible to the implementation of 'reduce'.
proc _cast(type t: Foo, const x: int)  return new Foo(x);
proc *(a: Foo, b: Foo)                 return new Foo(a.x*b.x);

proc main() {
  writeln( * reduce for i in 1..2 do new Foo(i) );
}
