
class Foo {
  var x : int;
}

proc helper(const ref c : borrowed Foo) {
  writeln("Inside helper, c = ", c);
  writeln("is 'c' wide? ", __primitive("is wide pointer", c));
}

proc main() {
  var ownLoc = new owned Foo(42);
  var loc = ownLoc.borrow();
  var ownRem = new owned Foo(100);
  var rem = ownRem.borrow();
  helper(loc);

  on Locales[numLocales-1] {
    helper(rem);
  }

  // Without optimizations, this will return 'true'
  writeln("is 'loc' wide? ", __primitive("is wide pointer", loc));
}
