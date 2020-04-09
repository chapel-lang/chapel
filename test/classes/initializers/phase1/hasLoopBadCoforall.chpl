class Foo {
  var x: int;

  proc init(a: [] int) {
    // First off, this is a race
    // Second, you aren't allowed to initialize fields in loops in Phase 1
    // Third, you aren't allowed to initialize fields in parallel statements
    //   in Phase 1
    coforall val in a do
      x = val;
  }
}

var foo = new unmanaged Foo([1, 2, 3, 4]);
writeln(foo);
delete foo;
