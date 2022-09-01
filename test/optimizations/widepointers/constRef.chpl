
class Foo {
  var x : int;
}

proc helper(const ref c : borrowed Foo) {
  writeln("Inside helper, c = ", c);
  writeln("is 'c' wide? ", __primitive("is wide pointer", c));
}

proc main() {
  var loc = (new owned Foo(42)).borrow();
  var rem = (new owned Foo(100)).borrow();
  helper(loc);

  on Locales[numLocales-1] {
    helper(rem);
  }

  // Without optimizations, this will return 'true'
  writeln("is 'loc' wide? ", __primitive("is wide pointer", loc));
}
